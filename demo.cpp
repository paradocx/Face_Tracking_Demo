//#include <QCoreApplication>
//#include <stdio.h>
//#include <iostream>
#include <opencv2/opencv.hpp>
//
//using namespace std;
//
////对轮廓按面积降序排列
//bool biggerSort(cv::vector<cv::Point> v1, cv::vector<cv::Point> v2)
//{
//	return cv::contourArea(v1)>cv::contourArea(v2);
//}
//int main(int argc, char *argv[])
//{
//	QCoreApplication a(argc, argv);
//	//视频不存在，就返回
//	cv::VideoCapture cap("C:/Users/Xiaolong Yang/Desktop/test/x64/Debug/ten.avi");//使用视频
//	//cv::VideoCapture cap(0)   //打开摄像头
//	if (cap.isOpened() == false)
//		return 0;
//
//	//定义变量
//	int i;
//
//	cv::Mat frame;			//当前帧
//	cv::Mat foreground;		//前景
//	cv::Mat bw;				//中间二值变量
//	cv::Mat se;				//形态学结构元素
//
//							//用混合高斯模型训练背景图像
//	cv::BackgroundSubtractorMOG mog;
//	for (i = 0; i<100; ++i)
//	{
//		cout << "正在训练背景:" << i << endl;
//		cap >> frame;
//		if (frame.empty() == true)
//		{
//			cout << "视频帧太少，无法训练背景" << endl;
//			getchar();
//			return 0;
//		}
//		mog(frame, foreground, 0.01);
//	}
//
//	//目标外接框、生成结构元素（用于连接断开的小目标）
//	cv::Rect rt;
//	se = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
//
//	//统计目标直方图时使用到的变量
//	cv::vector<cv::Mat> vecImg;
//	cv::vector<int> vecChannel;
//	cv::vector<int> vecHistSize;
//	cv::vector<float> vecRange;
//	cv::Mat mask(frame.rows, frame.cols, cv::DataType<uchar>::type);
//	//变量初始化
//	vecChannel.push_back(0);
//	vecHistSize.push_back(32);
//	vecRange.push_back(0);
//	vecRange.push_back(180);
//
//	cv::Mat hsv;		//HSV颜色空间，在色调H上跟踪目标（camshift是基于颜色直方图的算法）
//	cv::MatND hist;		//直方图数组
//	double maxVal;		//直方图最大值，为了便于投影图显示，需要将直方图规一化到[0 255]区间上
//	cv::Mat backP;		//反射投影图
//	cv::Mat result;		//跟踪结果
//
//						//视频处理流程
//
//	cv::vector<cv::vector<cv::Point> > contours;
//	while (1)
//	{
//		//读视频
//		cap >> frame;
//		if (frame.empty() == true)
//			break;
//
//		//生成结果图
//		frame.copyTo(result);
//
//		//检测目标
//		mog(frame, foreground, 0.01);
//		cv::imshow("混合高斯检测前景", foreground);
//	//	cv::moveWindow("混合高斯检测前景", 400, 0);
//		//对前景进行中值滤波、形态学膨胀操作，以去除伪目标和接连断开的小目标（一个大车辆有时会断开成几个小目标）
//		cv::medianBlur(foreground, foreground, 5);
//		cv::imshow("中值滤波", foreground);
//	//	cv::moveWindow("中值滤波", 800, 0);
//		cv::morphologyEx(foreground, foreground, cv::MORPH_DILATE, se);
//
//		//检索前景中各个连通分量的轮廓
//		foreground.copyTo(bw);
//		//cv::vector<cv::vector<cv::Point> > contours;
//		//contours.clear();
//		cv::findContours(bw, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
//		if (contours.size()<1)
//			continue;
//		//对连通分量进行排序
//		std::sort(contours.begin(), contours.end(), biggerSort);
//
//		//结合camshift更新跟踪位置（由于camshift算法在单一背景下，跟踪效果非常好；
//		//但是在监控视频中，由于分辨率太低、视频质量太差、目标太大、目标颜色不够显著
//		//等各种因素，导致跟踪效果非常差。  因此，需要边跟踪、边检测，如果跟踪不够好，
//		//就用检测位置修改
//		cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);
//		vecImg.clear();
//		vecImg.push_back(hsv);
//		for (int k = 0; k<contours.size(); ++k)
//		{
//			//第k个连通分量的外接矩形框
//			if (cv::contourArea(contours[k])<cv::contourArea(contours[0]) / 5)
//				break;
//			rt = cv::boundingRect(contours[k]);
//			//mask=0;
//			mask(rt) = 255;
//
//			//统计直方图
//			cv::calcHist(vecImg, vecChannel, mask, hist, vecHistSize, vecRange);
//			cv::minMaxLoc(hist, 0, &maxVal);
//			hist = hist * 255 / maxVal;
//			//计算反向投影图
//			cv::calcBackProject(vecImg, vecChannel, hist, backP, vecRange, 1);
//			//camshift跟踪位置
//			cv::Rect search = rt;
//			cv::RotatedRect rrt = cv::CamShift(backP, search, cv::TermCriteria(cv::TermCriteria::COUNT + cv::TermCriteria::EPS, 10, 1));
//			cv::Rect rt2 = rrt.boundingRect();
//			rt &= rt2;
//
//			//跟踪框画到视频上
//			cv::rectangle(result, rt, cv::Scalar(0, 255, 0), 2);
//		}
//
//		//结果显示
//		cv::imshow("Origin", frame);
//	//	cv::moveWindow("Origin", 0, 0);
//
//		cv::imshow("膨胀运算", foreground);
//	//	cv::moveWindow("膨胀运算", 0, 300);
//
//		cv::imshow("反向投影", backP);
//	//	cv::moveWindow("反向投影", 400, 300);
//
//		cv::imshow("跟踪效果", result);
//	//	cv::moveWindow("跟踪效果", 800, 300);
//		cv::waitKey(30);
//	}
//
//	getchar();
//	//  return 0;
//
//	return a.exec();
//}






#include <facedetectcnn.h>

#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QPainter>
#include <QGraphicsView>



QImage cvMat2QImage(const cv::Mat& mat)
{
	// 8-bits unsigned, NO. OF CHANNELS = 1
	if (mat.type() == CV_8UC1)
	{
		QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
		// Set the color table (used to translate colour indexes to qRgb values)
		image.setColorCount(256);
		for (int i = 0; i < 256; i++)
		{
			image.setColor(i, qRgb(i, i, i));
		}
		// Copy input Mat
		uchar *pSrc = mat.data;
		for (int row = 0; row < mat.rows; row++)
		{
			uchar *pDest = image.scanLine(row);
			memcpy(pDest, pSrc, mat.cols);
			pSrc += mat.step;
		}
		return image;
	}
	// 8-bits unsigned, NO. OF CHANNELS = 3
	else if (mat.type() == CV_8UC3)
	{
		// Copy input Mat
		const uchar *pSrc = (const uchar*)mat.data;
		// Create QImage with same dimensions as input Mat
		QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
		return image.rgbSwapped();
	}
	else if (mat.type() == CV_8UC4)
	{
		
		// Copy input Mat
		const uchar *pSrc = (const uchar*)mat.data;
		// Create QImage with same dimensions as input Mat
		QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
		return image.copy();
	}
	else
	{
	
		return QImage();
	}
}


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QImage srcImage;
	// 创建窗口
	QWidget widget;
	// 添加控件
	QGraphicsScene* gsViewScene = new QGraphicsScene();
	gsViewScene->setItemIndexMethod(QGraphicsScene::NoIndex);
	QGraphicsView* gvImageView = new QGraphicsView(&widget);
	gvImageView->setScene(gsViewScene);

	QLineEdit* leImagePath = new QLineEdit(&widget);
	QPushButton* pbSelectFile = new QPushButton(QStringLiteral("选择文件"), &widget);
	QPushButton* pbOpenCamera = new QPushButton(QStringLiteral("打开摄像头"), &widget);
	QPushButton* pbRunDetect = new QPushButton(QStringLiteral("执行检测"), &widget);
	pbRunDetect->setEnabled(false);
	QHBoxLayout* hbLayout = new QHBoxLayout;
	// 设置布局
	hbLayout->addWidget(leImagePath);
	hbLayout->addWidget(pbSelectFile);
	hbLayout->addWidget(pbOpenCamera);
	hbLayout->addWidget(pbRunDetect);
	QVBoxLayout* vbLayout = new QVBoxLayout(&widget);
	vbLayout->addLayout(hbLayout);
	vbLayout->addWidget(gvImageView);
	cv::Mat frame;

	// 添加处理操作
	QObject::connect(pbSelectFile, &QPushButton::clicked,
		[frame,leImagePath, gvImageView, gsViewScene, pbRunDetect, &srcImage, &widget]()
	{
		static QString path;
		path = QFileDialog::getOpenFileName(&widget,
			QStringLiteral("选择文件"),
			path, "Images(*.png *.jpg);;Videos(*.avi *.cam)");
		if (path.isEmpty()) { return; }
		
		if (path.right(3) == "png" || path.right(3) == "jpg") {     
			
			//检测单张图像
			QImage image;

			if (!image.load(path)) { return; }
                     //image= QImage((uchar*)frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
					// 图像太大的时候，执行太慢，先缩小一点
					//if (image.width() > 2048) { image = image.scaledToWidth(2048); }
					//if (image.width() > 1536) { image = image.scaledToWidth(1536); }

			widget.resize(image.width(), image.height());
			srcImage.swap(image);
			leImagePath->setText(path);

			gsViewScene->clear();
			gsViewScene->setSceneRect(srcImage.rect());
			gsViewScene->addPixmap(QPixmap::fromImage(srcImage));
			gvImageView->fitInView(gvImageView->sceneRect());

			pbRunDetect->setEnabled(true);
		   }
		else { 
			
			cv::Mat frame;			       //当前帧
			if (path.right(3) == "cam")
			{
				cv::VideoCapture cap(0);   //打开摄像头
				if (cap.isOpened() == false)
					return;
				while (1) {
					cap >> frame;
					if (frame.empty() == true)
						break;
					QImage image = cvMat2QImage(frame);
					widget.resize(image.width(), image.height());
					srcImage.swap(image);
					leImagePath->setText(path);

					gsViewScene->clear();
					gsViewScene->setSceneRect(srcImage.rect());
					gsViewScene->addPixmap(QPixmap::fromImage(srcImage));
					gvImageView->fitInView(gvImageView->sceneRect());
					cv::waitKey(30);
					pbRunDetect->setEnabled(true);

				}
				
			}
			else if (path.right(3) == "avi")
			{
				cv::VideoCapture cap(string((const char *)path.toLocal8Bit()));
				if (cap.isOpened() == false)
					return;
				while (1) {
					cap >> frame;
					if (frame.empty() == true)
						break;
					QImage image = cvMat2QImage(frame);
					widget.resize(image.width(), image.height());
					srcImage.swap(image);
					leImagePath->setText(path);

					gsViewScene->clear();
					gsViewScene->setSceneRect(srcImage.rect());
					gsViewScene->addPixmap(QPixmap::fromImage(srcImage));
					gvImageView->fitInView(gvImageView->sceneRect());
					cv::waitKey(30);
					pbRunDetect->setEnabled(true);

				}
			}
			
			
		}
		
		
		
		
		
		
	});

	QObject::connect(pbRunDetect, &QPushButton::clicked,
		[gvImageView, gsViewScene, &srcImage]
	{
		// 转换为RGB24
		QImage image = srcImage.convertToFormat(QImage::Format_RGB888);
		int rows = image.height();
		int cols = image.width();
		int rowbytes = image.bytesPerLine();

		uchar* pImgData = image.bits();
		// RGB -> BGR 因为facedetect_cnn函数要求传入图像为BGR三波段图像
		for (int r = 0; r < rows; ++r) {
			uchar* pRow = pImgData + (r * rowbytes);
			for (int c = 0; c < cols; ++c) {
				qSwap(pRow[c * 3], pRow[c * 3 + 2]);
			}
		}
		// 进行检测
		QByteArray buffer(0x20000, 0);
		int* pResults = facedetect_cnn((uchar*)buffer.data(), pImgData, cols, rows, rowbytes);
		// 将检测结果画到图像上
		QPixmap pixmap = QPixmap::fromImage(srcImage);
		if (pResults != NULL) {
			for (int i = 0; i< *pResults; ++i) {
				short * p = ((short*)(pResults + 1)) + 142 * i;
				int x = p[0];
				int y = p[1];
				int w = p[2];
				int h = p[3];
				int confidence = p[4];
				
				// 查看源码可知，其并未进行人像框范围和置信率以外的数据赋值

				if (confidence < 60)   //仅输出置信度60%以上
					continue;
				QPainter painter(&pixmap);
				// painter.setBrush(QBrush(QColor(255,0,0),));
				painter.setPen(Qt::green);
				// painter.drawEllipse(x,y,w,h);
				painter.drawRect(x, y, w, h);
				painter.drawText(x, y, w, h, Qt::AlignHCenter,
					QStringLiteral("置信:%1%").arg(confidence));
			}
		}

		gsViewScene->clear();
		gsViewScene->setSceneRect(pixmap.rect());
		gsViewScene->addPixmap(pixmap);
		gvImageView->fitInView(gvImageView->sceneRect());
	});


	
	widget.show();
	return a.exec();
}
