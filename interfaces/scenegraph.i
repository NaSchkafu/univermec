%{
#include "stable.h"
#include "ui/scenegraph.hpp"
%}

%include "ui/scenegraph.hpp"

%inline %{
  std::istream& open_stream(const char *filename) {
    std::istream *infile = new std::ifstream(filename);
    return *infile;
  }
%}

std::istream& open_stream(const char *filename);

%template(ShVector) std::vector<ui::scene_handle_t>;
