#ifndef SOURCE_CONTROLLER_H
#define SOURCE_CONTROLLER_H

#include "components/pkb/pkb_declarations.h"
#include "components/source_subsystem/source_declarations.h"

class PKB;

namespace source {

class SourceController {
 public:
  [[nodiscard]] static String RetrieveFileContent(String &file_path);
  static void Tokenize(String &source, TokenStream &token_stream);
  [[nodiscard]] static ProgramNodePtr ParseTokenStream(TokenStream &token_stream);
  static void PopulatePKB(pkb::PkbPtr &pkb, ProgramNodePtr &ast);
};

}

#endif //SOURCE_CONTROLLER_H
