#pragma once
#include <QtCore>
#include <qpolygon.h>
#include <opencv2\core\types.hpp>
#include <opencv2\imgcodecs.hpp>

/// <summary>
/// 分类配置
/// </summary>
class Coconames {
public:
	Coconames();
	int classid;///CLASSID
	bool checked;///是否识别
	QString className;///英文名
	QString className1;///中文名
	float confidence;///置信度阈值
public:
	QHash<int, Coconames> coconames;

	/// <summary>
	/// 2\输入：80类可选配置文件读入；{{id,bool},{bool,id},{bool,id,classname}...}
	/// 备注：对话框配置来配置上述配置文件。目的是提供配置界面来配置这边的读入配置文件。
	/// 
	/// 
	/// 配置文件采用ascii编码格式的文本文件。
	/// 配置文件格式：
	/// 是否识别    类别ID      英文名称		中文名称	置信度		
	/// 1		    0		    person  		人		    0.5
	/// 0		    1		    bicycle	    	自行车		0.7
	/// 1		    2		    car		        小汽车		0.9
	/// 0		    3		    motor  bike		摩托车		0.5
	/// ...
	/// </summary>
	/// <param name="cocoFilename">QString 配置文件路径。</param>
	/// <returns>
	/// QHash 容器[classid,Cococname]
	/// </returns>
	QHash<int, Coconames> loadCoconames(const QString cocoFilename);

	bool inCoconames(int classid);

	QString formatedOutput(
		cv::Mat& currentFrame,
		std::vector<cv::Rect>& out_Boxes,
		std::vector<int>& out_ClassIds,
		std::vector<float>& out_Confidences,
		std::vector<cv::String>& out_ClassNames);

	std::string base64Decode(const char* Data, int DataByte);
	std::string base64Encode(const unsigned char* Data, int DataByte);
	std::string Mat2Base64(const cv::Mat& img, std::string imgType);
	cv::Mat Base2Mat(std::string& base64_data);


};

struct Vertex
{
	int devid;
	int polyid;
	int vertexid;
	int x;
	int y;

};
class DevPolygons {
public:
	DevPolygons();
public:
	int deviceid;
	int polyonid;
	int vertexid;
	int x;
	int y;

public:
	QHash<QString, QPolygonF> polygons;

	/// <summary>
	/// 3\输入：各个雷达的多变行QLIST向量组合；{{CAMERAID,[PONT1,PONT2,PONT3,PON4]},{CAMERAID,[PONT1,PONT2,PONT3,PON4]}，。。。16个}
	/// 备注：
	/// 
	/// 配置文件采用ascii编码格式的文本文件。
	/// 配置文件格式：
	/// 所属设备ID  PolygonID   顶点序号    X坐标     Y坐标
	/// 1		    0		    1           100       100
	/// 1		    0		    2           100       100
	/// 2		    2		    1           100       100
	/// 3		    3		    1           100       100
	/// </summary>
	/// <param name="polygonFilename">QString 配置文件路径。</param>
	/// <returns>///[设备id.polyid,QPolygonF]</returns>
	QHash<QString, QPolygonF> loadPolygons(const QString polygonFilename);

	bool inPolygons(int x, int y);
};

