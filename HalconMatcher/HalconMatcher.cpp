#include "HalconMatcher.h"
using namespace HDevEngineCpp;

HMatcher::HMatcher()
{

}

HMatcher::~HMatcher()
{
	try
	{
	}
	catch (const std::exception&)
	{
			
	}

}

void HMatcher::doMatching(HObject& imageTemplate, HObject& image, HTuple& rows, HTuple& cols, HTuple& angles, HTuple& scales, HTuple& scores)
{
	try
	{
		long t1 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		HDevEngine	halconEngine = HDevEngine();
		halconEngine.SetProcedurePath("C:/Users/Admin/source/repos/DNNCV/HalconMatcher");
		HDevProcedure	proc = HDevProcedure("scaled_shape_match");
		HDevProcedureCall	proc_call = HDevProcedureCall(proc);

		proc_call.SetInputIconicParamObject(1, imageTemplate);
		proc_call.SetInputIconicParamObject(2, image);
		proc_call.Execute();
		rows = proc_call.GetOutputCtrlParamTuple(1);
		cols = proc_call.GetOutputCtrlParamTuple(2);
		angles = proc_call.GetOutputCtrlParamTuple(3);
		scales = proc_call.GetOutputCtrlParamTuple(4);
		scores = proc_call.GetOutputCtrlParamTuple(5);
		long t2 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		std::cout << 
			"\trows:" << rows[0].D() << "," <<
			"\tcols:" << cols[0].D() << "," <<
			"\tangles:" << angles[0].D() << "," <<
			"\tscales:" << scales[0].D() << "," <<
			"\tscores:" << scores[0].D() << "," <<
			"\ttime elapsed:" << t2 - t1 << "ms" << std::endl;
		proc_call.Reset();
	}
	catch (const std::exception&)
	{

	}

}
