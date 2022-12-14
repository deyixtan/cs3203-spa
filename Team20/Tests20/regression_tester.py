import os
import subprocess
import sys
import xml.etree.ElementTree as ET

def check_env():
    does_out_exist = os.path.isfile("out.xml")
    does_analysis_exist = os.path.isfile("analysis.xsl")

    if not does_out_exist or not does_analysis_exist:
        return False
    return True


def find_tests():
    test_paths = []
    for subdir, test, files in os.walk('.'):
        if subdir.startswith("./ignore"):
            continue
        for file in files:
            if file.endswith('source.txt'):
                query_file_name = file[:-10] + "queries.txt"
                if query_file_name in files:
                    source_file_path = os.path.join(subdir, file)
                    query_file_path = os.path.join(subdir, query_file_name)
                    test_paths.append((source_file_path, query_file_path))

    return test_paths


def analyse():
    tree = ET.parse("out.xml")
    queries = tree.getroot()[1]

    correct_list = []
    wrong_list = []
    timeout_list = []
    for query in queries:
        query_id = query[0].text
        tag_type = query[2].tag

        # check for errors
        if tag_type == "exception":
            wrong_list.append(query_id)
            continue
        elif tag_type == "timeout":
            timeout_list.append(query_id)
            continue

        is_result_tag = query[5].tag
        if is_result_tag == "failed":
            wrong_list.append(query_id)
            continue
        correct_list.append(query_id)

    return correct_list, wrong_list, timeout_list


def execute_tests(autotester_path, test_files):
    total_correct_count = 0
    total_wrong_count = 0
    total_timeout_count = 0

    for source_path, query_path in test_files:
        subprocess.run([autotester_path, source_path, query_path, "out.xml"], stdout=subprocess.DEVNULL)
        
        try:
            correct_list, wrong_list, timeout_list = analyse()
        except ET.ParseError:
            raise RuntimeError(f"[{source_path[source_path.rfind('/') + 1:-11]}] Unable to analyse test results.")

        correct_count = len(correct_list)
        wrong_count = len(wrong_list)
        timeout_count = len(timeout_list)
        total = correct_count + wrong_count + timeout_count
        total_correct_count += correct_count
        total_wrong_count += wrong_count
        total_timeout_count += timeout_count
        print(f"[{source_path[source_path.rfind('/') + 1:-11]}] {correct_count}/{total} test cases.")
        if wrong_count > 0:
            print(f"\tWrong cases: {wrong_list}")
        if timeout_count > 0:
            print(f"\tTimeout cases: {timeout_list}")

    return total_correct_count, total_wrong_count, total_timeout_count


if __name__ == "__main__":
    if not check_env:
        print("Environment not set up properly.")
        sys.exit(1)

    try:
        test_files = find_tests()
        correct_count, wrong_count, timeout_count = execute_tests(sys.argv[1], test_files)
        total = correct_count + wrong_count + timeout_count

        print(f"Correct test cases: {correct_count}/{total}.")
        print(f"Wrong test cases: {wrong_count + timeout_count}/{total}.")

        if (wrong_count > 0) or (timeout_count > 0):
            sys.exit(1)
        sys.exit(0)
    except Exception as e:
        print(e)
        sys.exit(1)
