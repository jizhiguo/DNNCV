#include "CocoHelper.h"

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
	Coconames c;


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

	result = QString(u8"图片的BASE64：%1，防区内的识别目标信息：%2")
		.arg(c.Mat2Base64(currentFrame, "jpg").c_str())
		.arg(str);
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

std::string Coconames::base64Decode(const char* Data, int DataByte)
{
	//解码表
	const char DecodeTable[] =
	{
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		62, // '+'
		0, 0, 0,
		63, // '/'
		52, 53, 54, 55, 56, 57, 58, 59, 60, 61, // '0'-'9'
		0, 0, 0, 0, 0, 0, 0,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
		13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, // 'A'-'Z'
		0, 0, 0, 0, 0, 0,
		26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
		39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, // 'a'-'z'
	};
	//返回值
	std::string strDecode;
	int nValue;
	int i = 0;
	while (i < DataByte)
	{
		if (*Data != '\r' && *Data != '\n')
		{
			nValue = DecodeTable[*Data++] << 18;
			nValue += DecodeTable[*Data++] << 12;
			strDecode += (nValue & 0x00FF0000) >> 16;
			if (*Data != '=')
			{
				nValue += DecodeTable[*Data++] << 6;
				strDecode += (nValue & 0x0000FF00) >> 8;
				if (*Data != '=')
				{
					nValue += DecodeTable[*Data++];
					strDecode += nValue & 0x000000FF;
				}
			}
			i += 4;
		}
		else// 回车换行,跳过
		{
			Data++;
			i++;
		}
	}
	return strDecode;
}

std::string Coconames::base64Encode(const unsigned char* Data, int DataByte)
{
	//编码表
	const char EncodeTable[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	//返回值
	std::string strEncode;
	unsigned char Tmp[4] = { 0 };
	int LineLength = 0;
	for (int i = 0; i < (int)(DataByte / 3); i++)
	{
		Tmp[1] = *Data++;
		Tmp[2] = *Data++;
		Tmp[3] = *Data++;
		strEncode += EncodeTable[Tmp[1] >> 2];
		strEncode += EncodeTable[((Tmp[1] << 4) | (Tmp[2] >> 4)) & 0x3F];
		strEncode += EncodeTable[((Tmp[2] << 2) | (Tmp[3] >> 6)) & 0x3F];
		strEncode += EncodeTable[Tmp[3] & 0x3F];
		if (LineLength += 4, LineLength == 76) { strEncode += "\r\n"; LineLength = 0; }
	}
	//对剩余数据进行编码
	int Mod = DataByte % 3;
	if (Mod == 1)
	{
		Tmp[1] = *Data++;
		strEncode += EncodeTable[(Tmp[1] & 0xFC) >> 2];
		strEncode += EncodeTable[((Tmp[1] & 0x03) << 4)];
		strEncode += "==";
	}
	else if (Mod == 2)
	{
		Tmp[1] = *Data++;
		Tmp[2] = *Data++;
		strEncode += EncodeTable[(Tmp[1] & 0xFC) >> 2];
		strEncode += EncodeTable[((Tmp[1] & 0x03) << 4) | ((Tmp[2] & 0xF0) >> 4)];
		strEncode += EncodeTable[((Tmp[2] & 0x0F) << 2)];
		strEncode += "=";
	}


	return strEncode;
}

std::string Coconames::Mat2Base64(const cv::Mat& img, std::string imgType)
{
	//Mat转base64
	std::string img_data;
	std::vector<uchar> vecImg;
	std::vector<int> vecCompression_params;
	vecCompression_params.push_back(9);
	//vecCompression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
	vecCompression_params.push_back(90);
	imgType = "." + imgType;
	cv::imencode(imgType, img, vecImg, vecCompression_params);
	img_data = base64Encode(vecImg.data(), vecImg.size());
	return img_data;
}

cv::Mat Coconames::Base2Mat(std::string& base64_data)
{
	cv::Mat img;
	std::string s_mat;
	s_mat = base64Decode(base64_data.data(), base64_data.size());
	std::vector<char> base64_img(s_mat.begin(), s_mat.end());
	img = cv::imdecode(base64_img, 0);
	//img = cv::imdecode(base64_img, CV_LOAD_IMAGE_COLOR);
	return img;
}
