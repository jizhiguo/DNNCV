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

	int rowCnt = fileContent.count(); //�ı���������1���Ǳ���
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
			str = QString(u8"���:%1������:%2;%3")
			.arg(out_ClassNames[idx].c_str())
			.arg(item.value())
			.arg(str);
	}

	result = QString(u8"ͼƬ��BASE64��%1�������ڵ�ʶ��Ŀ����Ϣ��%2").arg(str_encode.c_str()).arg(str);
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

	int rowCnt = fileContent.count(); //�ı���������1���Ǳ���
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
