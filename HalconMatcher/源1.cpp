#include "HalconCpp.h"
#include "HDevEngineCpp.h"

using namespace HalconCpp;
using namespace HDevEngineCpp;


int main(int argc, char* argv[])
{
    HDevEngine my_engine;
    std::string halcon_examples =
        (std::string)HSystem::GetSystem("example_dir")[0].S();
    std::string program_path(halcon_examples), ext_proc_path(halcon_examples);

    program_path += "/hdevengine/hdevelop/fin_detection.hdev";
    ext_proc_path += "/hdevengine/procedures";

    my_engine.SetProcedurePath(ext_proc_path.c_str());

    HDevProgram my_program;
    try
    {
        my_program.LoadProgram(program_path.c_str());
    }
    catch (HDevEngineException& hdev_exception)
    {

    }

    HDevProgramCall prog_call = my_program.Execute();
    HTuple result = prog_call.GetCtrlVarTuple("FinArea");
    printf("\nFin Area: %f\n\n", result[0].D());


    return 0;
}
