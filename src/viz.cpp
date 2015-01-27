
#define EMPTY_VIZ_TRAVERSAL        0
#define DEFAULT_VIZ_TRAVERSAL      1
#define PROGRAMMABLE_VIZ_TRAVERSAL 2

#ifndef VIZ_TRAVERSAL
# error Visualization traversal was not specified!
#endif

#if   VIZ_TRAVERSAL == EMPTY_VIZ_TRAVERSAL
# include "Viz/Traversals/empty.hpp"
#elif VIZ_TRAVERSAL == DEFAULT_VIZ_TRAVERSAL
# include "Viz/Traversals/default.hpp"
#elif VIZ_TRAVERSAL == PROGRAMMABLE_VIZ_TRAVERSAL
# include "Viz/Traversals/programmable.hpp"
#else
# error Unknown visualization traversal!
#endif

int main(int argc, char ** argv) {
  CommandlineProcessing::extraCppSourceFileSuffixes.push_back("h");
  CommandlineProcessing::extraCppSourceFileSuffixes.push_back("hpp");

  std::vector<std::string> args;
  std::string prefix_opt("-prefix");
  std::string prefix;

#if VIZ_TRAVERSAL == PROGRAMMABLE_VIZ_TRAVERSAL
  std::string viz_program_file_opt("-viz_program");
  std::string viz_program_file;
#endif

  for (int i = 0; i < argc; i++) {
    if (prefix_opt == argv[i]) {
      i++;
      prefix = argv[i];
    }
#if VIZ_TRAVERSAL == PROGRAMMABLE_VIZ_TRAVERSAL
    else if (viz_program_file_opt == argv[i]) {
      i++;
      viz_program_file = argv[i];
    }
#endif
    else {
      args.push_back(argv[i]);
    }
  }

  SgProject * project = new SgProject::SgProject(args);


#if   VIZ_TRAVERSAL == EMPTY_VIZ_TRAVERSAL
  Rose::Viz::Traversal<Rose::Viz::Traversals::Empty> traversal;
#elif VIZ_TRAVERSAL == DEFAULT_VIZ_TRAVERSAL
  Rose::Viz::Traversal<Rose::Viz::Traversals::Default> traversal;
#elif VIZ_TRAVERSAL == PROGRAMMABLE_VIZ_TRAVERSAL
  Rose::Viz::Traversals::Programmable::Program program(viz_program_file);
  Rose::Viz::Traversal<Rose::Viz::Traversals::Programmable> traversal(program);
#else
# error Unknown visualization traversal!
#endif

  std::vector<SgSourceFile *> files = SageInterface::querySubTree<SgSourceFile>(project);
  std::vector<SgSourceFile *>::iterator it_file;
  for (it_file = files.begin(); it_file != files.end(); it_file++) {
    std::string filename = (*it_file)->get_sourceFileNameWithoutPath();
    size_t dotpos = filename.find_last_of('.');
    std::string basename = filename.substr(0, dotpos);
    std::string extension = filename.substr(dotpos+1);
    std::cerr << "basename = " << basename << ", prefix = " << prefix << ", ext = " << extension << std::endl;

    traversal.traverseWithinFile(*it_file, traversal.makeAttribute(basename));

    std::ostringstream oss;
    oss << prefix << basename << ".dot";

    std::ofstream dot_file;
    dot_file.open(oss.str().c_str());
    assert(dot_file.is_open());

    traversal.toGraphViz(basename, dot_file);

    dot_file.close();
  }

  return 0;
}

