#include "nobpp.hpp"

int main() {
    nobpp::CommandBuilder()
        .set_project_name("http.cpp2")
        .set_compiler(nobpp::Compiler::clang)
        .set_language(nobpp::Language::cpp)
        .set_target_os(nobpp::TargetOS::windows)
        .add_option("-DLOG_LEVEL_INFO")
        .add_file("./src/main.cpp")
        .add_option("-std=c++2c")
        .set_optimization_level(nobpp::OptimizationLevel::o3)
        .set_build_dir("build")
        .set_output("main")
        .run();
}