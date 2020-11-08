#pragma once
#include <QtCore>
#include <qpolygon.h>
#include <opencv2\core\types.hpp>
#include <opencv2\imgcodecs.hpp>

/// <summary>
/// ��������
/// </summary>
class Coconames {
public:
	Coconames();
	int classid;///CLASSID
	bool checked;///�Ƿ�ʶ��
	QString className;///Ӣ����
	QString className1;///������
	float confidence;///���Ŷ���ֵ
public:
	QHash<int, Coconames> coconames;

	/// <summary>
	/// 2\���룺80���ѡ�����ļ����룻{{id,bool},{bool,id},{bool,id,classname}...}
	/// ��ע���Ի����������������������ļ���Ŀ�����ṩ���ý�����������ߵĶ��������ļ���
	/// 
	/// 
	/// �����ļ�����ascii�����ʽ���ı��ļ���
	/// �����ļ���ʽ��
	/// �Ƿ�ʶ��    ���ID      Ӣ������		��������	���Ŷ�		
	/// 1		    0		    person  		��		    0.5
	/// 0		    1		    bicycle	    	���г�		0.7
	/// 1		    2		    car		        С����		0.9
	/// 0		    3		    motor  bike		Ħ�г�		0.5
	/// ...
	/// </summary>
	/// <param name="cocoFilename">QString �����ļ�·����</param>
	/// <returns>
	/// QHash ����[classid,Cococname]
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
	/// 3\���룺�����״�Ķ����QLIST������ϣ�{{CAMERAID,[PONT1,PONT2,PONT3,PON4]},{CAMERAID,[PONT1,PONT2,PONT3,PON4]}��������16��}
	/// ��ע��
	/// 
	/// �����ļ�����ascii�����ʽ���ı��ļ���
	/// �����ļ���ʽ��
	/// �����豸ID  PolygonID   �������    X����     Y����
	/// 1		    0		    1           100       100
	/// 1		    0		    2           100       100
	/// 2		    2		    1           100       100
	/// 3		    3		    1           100       100
	/// </summary>
	/// <param name="polygonFilename">QString �����ļ�·����</param>
	/// <returns>///[�豸id.polyid,QPolygonF]</returns>
	QHash<QString, QPolygonF> loadPolygons(const QString polygonFilename);

	bool inPolygons(int x, int y);
};

