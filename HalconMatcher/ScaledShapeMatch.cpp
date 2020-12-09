/*****************************************************************************
 * File generated by HDevelop for Halcon/Cpp Version 17.12
 *
 * Do not modify!
 *****************************************************************************/

#include "ScaledShapeMatch.h"

#include <algorithm>
#include <map>
#include <mutex>
#include <string>

#ifndef __APPLE__
  #include <hdevengine/HDevEngineCpp.h>
#else
  #include <HDevEngineCpp/HDevEngineCpp.h>
#endif

using namespace HalconCpp;
using namespace HDevEngineCpp;

namespace ScaledShapeMatch {

  std::string sgResourcePath;
  
  bool AddResourcePathToProcedurePath()
  {
    HDevEngineCpp::HDevEngine().AddProcedurePath(sgResourcePath.c_str());
    return true;
  }

  bool LazyInitProcedurePath()
  {
    static std::mutex lock;
    std::unique_lock<std::mutex> locker(lock);
    static const bool init = AddResourcePathToProcedurePath();
    return init;
  }

  void SetResourcePath(const char* resource_path)
  {
    sgResourcePath = resource_path;
    std::replace(sgResourcePath.begin(),sgResourcePath.end(), '\\','/');
    if(sgResourcePath.length() > 0 && sgResourcePath[sgResourcePath.length()-1]!='/')
    {
      sgResourcePath+="/";
    }
    AddResourcePathToProcedurePath();
  }

  template <typename T>
  struct ParamHandler
  {
  };

  template <>
  struct ParamHandler<HalconCpp::HTuple>
  {
    static void SetParameter(HDevEngineCpp::HDevProcedureCall& proc,
        const char*                                     name,
        HalconCpp::HTuple const&                        parameter)
    {
      proc.SetInputCtrlParamTuple(name, parameter);
    }

    static HalconCpp::HTuple GetParameter(
        HDevEngineCpp::HDevProcedureCall& proc, const char* name)
    {
      return proc.GetOutputCtrlParamTuple(name);
    }
  };

  template <>
  struct ParamHandler<HalconCpp::HObject>
  {
    static void SetParameter(HDevEngineCpp::HDevProcedureCall& proc,
        const char*                                     name,
        HalconCpp::HObject const&                       parameter)
    {
      proc.SetInputIconicParamObject(name, parameter);
    }

    static HalconCpp::HObject GetParameter(
        HDevEngineCpp::HDevProcedureCall& proc, const char* name)
    {
      return proc.GetOutputIconicParamObject(name);
    }
  };

  template <>
  struct ParamHandler<HalconCpp::HTupleVector>
  {
    static void SetParameter(HDevEngineCpp::HDevProcedureCall& proc,
        const char*                                     name,
        HalconCpp::HTupleVector const&                  parameter)
    {
      proc.SetInputCtrlParamVector(name, parameter);
    }

    static HalconCpp::HTupleVector GetParameter(
        HDevEngineCpp::HDevProcedureCall& proc, const char* name)
    {
      return proc.GetOutputCtrlParamVector(name);
    }
  };

  template <>
  struct ParamHandler<HalconCpp::HObjectVector>
  {
    static void SetParameter(HDevEngineCpp::HDevProcedureCall& proc,
        const char*                                     name,
        HalconCpp::HObjectVector const&                 parameter)
    {
      proc.SetInputIconicParamVector(name, parameter);
    }

    static HalconCpp::HObjectVector GetParameter(
        HDevEngineCpp::HDevProcedureCall& proc, const char* name)
    {
      return proc.GetOutputIconicParamVector(name);
    }
  };


  HDevProgram GetProgram(std::string const& program_file)
  {
    static std::mutex lock;
    static std::map<std::string,HDevProgram> programs;

    std::unique_lock<std::mutex> locker(lock);

    auto prog_iter = programs.find(program_file);
    if(prog_iter != programs.end())
    {
      return prog_iter->second;
    }
    else
    {
      HDevProgram program(program_file.c_str());
      programs[program_file] = program;
      return program;
    }
    return HDevProgram();
  }

  void createTemplateImg(
    HalconCpp::HObject const& Image,
    HalconCpp::HTuple* ModelID)
  {     
    static HDevEngineCpp::HDevProcedure procedure(GetProgram(sgResourcePath+"scaled_shape_match.hdev"),"createTemplateImg");
    HDevEngineCpp::HDevProcedureCall call=procedure;
    ParamHandler<HalconCpp::HObject>::SetParameter(call,"Image",Image);
        
    call.Execute();

    if (ModelID != nullptr)
      * ModelID = ParamHandler<HalconCpp::HTuple>::GetParameter(call,"ModelID");
  }

  void scaled_shape_match(
    HalconCpp::HObject const& Image,
    HalconCpp::HTuple const& ModelID,
    HalconCpp::HTuple* Row,
    HalconCpp::HTuple* Column,
    HalconCpp::HTuple* Angle,
    HalconCpp::HTuple* Scale,
    HalconCpp::HTuple* Score)
  {     
    static HDevEngineCpp::HDevProcedure procedure(GetProgram(sgResourcePath+"scaled_shape_match.hdev"),"scaled_shape_match");
    HDevEngineCpp::HDevProcedureCall call=procedure;
    ParamHandler<HalconCpp::HObject>::SetParameter(call,"Image",Image);
    ParamHandler<HalconCpp::HTuple>::SetParameter(call,"ModelID",ModelID);
        
    call.Execute();

    if (Row != nullptr)
      * Row = ParamHandler<HalconCpp::HTuple>::GetParameter(call,"Row");
    if (Column != nullptr)
      * Column = ParamHandler<HalconCpp::HTuple>::GetParameter(call,"Column");
    if (Angle != nullptr)
      * Angle = ParamHandler<HalconCpp::HTuple>::GetParameter(call,"Angle");
    if (Scale != nullptr)
      * Scale = ParamHandler<HalconCpp::HTuple>::GetParameter(call,"Scale");
    if (Score != nullptr)
      * Score = ParamHandler<HalconCpp::HTuple>::GetParameter(call,"Score");
  }

};

