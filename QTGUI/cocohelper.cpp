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

Coconames::Coconames()
{
}

QHash<int, Coconames>  Coconames::loadCoconames(const QString cocoFilename)
{
	QHash<int, Coconames> coconames;
	QFile file(cocoFilename);
	if (!file.exists()) return coconames;
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return coconames;

	QStringList fileContent;
	QTextStream textStream(&file);
	while (!textStream.atEnd())
	{
		QString str = textStream.readLine();
		fileContent.append(str.toUtf8());
	}
	file.close();

	int rowCnt = fileContent.count(); //文本行数，第1行是标题
	QString aText;
	QStringList tmpList;
	Coconames coco;
	for (int i = 1; i < rowCnt; i++)
	{
		QString aLineText = fileContent.at(i);
		QStringList tmpList = aLineText.split(QRegExp("\\t+"), QString::SkipEmptyParts);
		coco.checked = tmpList.at(0).toInt() == 0 ? false : true;
		coco.classid = tmpList.at(1).toInt();
		coco.className = tmpList.at(2);
		coco.className1 = tmpList.at(3);
		coco.confidence = tmpList.at(4).toFloat();
		coconames.insert(coco.classid, coco);
	}
	this->coconames = coconames;

	return coconames;
}

bool Coconames::inCoconames(int classid)
{
	if (coconames.contains(classid))
		return  this->coconames.value(classid).checked;
	return false;
}

QString Coconames::formatedOutput(
	cv::Mat& currentFrame,
	std::vector<cv::Rect>& out_Boxes,
	std::vector<int>& out_ClassIds,
	std::vector<float>& out_Confidences,
	std::vector<cv::String>& out_ClassNames)
{
	QString result;

	//base64 encode
	std::vector<uchar> data_encode;
	cv::imencode(".png", currentFrame, data_encode);
	std::string str_encode(data_encode.begin(), data_encode.end());

	//count on classes
	QHash<int, int> counter;
	int ids = out_ClassIds.size();
	int tmpcount;
	for (size_t i = 0; i < ids; i++)
	{
		if (counter.contains(out_ClassIds[i]))
		{
			tmpcount = counter.value(out_ClassIds[i]);
			tmpcount += 1;
			counter.insert(out_ClassIds[i], tmpcount);
		}
		else
		{
			counter.insert(out_ClassIds[i], 1);
		}
	}
	QHashIterator<int, int > i(counter);
	QHashIterator<int, int >::Item item;
	QString str;
	int idx;
	while (i.hasNext())
	{
		item = i.next();

		std::vector<int>::iterator iter = std::find(out_ClassIds.begin(), out_ClassIds.end(), item.key());
		if (iter == out_ClassIds.end())
		{
			idx = -1;
		}
		else {
			idx = std::distance(out_ClassIds.begin(), iter);
		}
		if (idx)
			str = QString(u8"类别:%1，数量:%2;%3")
			.arg(out_ClassNames[idx].c_str())
			.arg(item.value())
			.arg(str);
	}

	result = QString(u8"图片的BASE64：%1，防区内的识别目标信息：%2").arg(str_encode.c_str()).arg(str);
	return result;
}

DevPolygons::DevPolygons()
{
}

QHash<QString, QPolygonF> DevPolygons::loadPolygons(const QString polygonFilename)
{
	QHash<QString, QPolygonF> polygons;
	QFile file(polygonFilename);
	if (!file.exists()) return polygons;
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return polygons;

	QStringList fileContent;
	QTextStream textStream(&file);
	while (!textStream.atEnd())
	{
		QString str = textStream.readLine();
		fileContent.append(str.toUtf8());
	}
	file.close();

	int rowCnt = fileContent.count(); //文本行数，第1行是标题
	QString aText;
	QStringList tmpList;
	QPolygonF tmpPolygon = QPolygonF();
	for (int i = 1; i < rowCnt; i++)
	{
		QString ployKey;
		Vertex vertex = Vertex();
		QString aLineText = fileContent.at(i);
		QStringList tmpList = aLineText.split(QRegExp("\\t+"), QString::SkipEmptyParts);
		vertex.devid = tmpList.at(0).toInt();
		vertex.polyid = tmpList.at(1).toInt();
		vertex.vertexid = tmpList.at(2).toInt();
		vertex.x = tmpList.at(3).toInt();
		vertex.y = tmpList.at(4).toInt();

		ployKey = QString("%1.%2").arg(vertex.devid).arg(vertex.polyid);
		if (polygons.contains(ployKey))
		{
			tmpPolygon = polygons.value(ployKey);
			tmpPolygon << QPointF(vertex.x, vertex.y);
			polygons.insert(ployKey, tmpPolygon);
		}
		else
		{
			tmpPolygon = QPolygonF();
			tmpPolygon << QPointF(vertex.x, vertex.y);
			polygons.insert(ployKey, tmpPolygon);
		}
	}
	this->polygons = polygons;
	return polygons;
}

bool DevPolygons::inPolygons(int x, int y)
{
	QListIterator<QPolygonF> i(polygons.values());
	while (i.hasNext())
	{
		if (i.next().containsPoint(QPointF(x, y), Qt::WindingFill)) return true;
	}
	return false;
}
