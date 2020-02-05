//解封装
#include <iostream>
#include <thread>
extern "C" {
#include "libavformat/avformat.h"
}
using namespace std;
#pragma comment(lib,"avformat.lib")
#pragma comment(lib,"avutil.lib")
#pragma comment(lib,"avcodec.lib")
static double r2d(AVRational r)
{
	return r.den == 0 ? 0:(double)r.num / (double)r.den;
}
void XSleep(int ms)
{
	//c++ 11
	chrono::milliseconds du(ms);
	this_thread::sleep_for(du);
}

int main(int argc, char* argv[])
{
	cout << "Test Demux FFmpeg.club" << endl;
	const char* path = "test.mp4";
	//初始化封装库
	av_register_all(); //这个方法在ffmepg4.0后不再推荐使用，直接注释即可，也可关闭sdl检查
	//初始化网络库（可以打开rtsp rtmp http协议的流媒体视频）
	avformat_network_init();
	
	//参数设置
	AVDictionary* opts = NULL;
	//设置rtsp流以tcp协议打开
	av_dict_set(&opts, "rstp_transport", "tcp", 0);
	//网络延时时间
	av_dict_set(&opts, "max_delay", "500", 0);

	//解封装上下文
	AVFormatContext* ic = NULL;
	int re = avformat_open_input(
		&ic,
		path,
		0,  // 0表示自动选择解封器
		&opts //参数设置，比如rtsp的延时时间
	);
	if (re != 0) //成功返回值为0
	{
		char buf[1024] = { 0 };
		av_strerror(re, buf, sizeof(buf) - 1);
		cout << "open " << path << " failed!:" << buf << endl;
		getchar();
		return -1;
	}
	cout << "open " << path << " success!" << endl;

	//获取流信息
	re = avformat_find_stream_info(ic, 0);

	//总时长 毫秒
	int total = ic->duration / (AV_TIME_BASE/1000);
	cout << "totalMs = " << total << endl;

	//打印视频流详细信息
	av_dump_format(ic, 0, path, 0); //第二个参数没用，第三个参数也没有影响，第四个参数：是否是输出。

	//音视频索引，读取时区分音视频
	int videoStream = 0;
	int audioStream = 1;

	//（①遍历；②函数获取）

	//获取音视频流信息 （遍历）
	for (int i = 0; i < ic->nb_streams; i++)  //nb_streams表示视音频流的个数
	{
		AVStream* as = ic->streams[i];
		cout << "codec_id = " << as->codecpar->codec_id << endl;
		cout << "format = " << as->codecpar->format << endl; //例如8表示FLTP（float 32位）平面存储方式，即通道顺序存储，32位需进行重采样
		//音频 AVMEDIA_TYPE_AUDIO
		if (as->codecpar->codec_type == AVMEDIA_TYPE_AUDIO)
		{
			audioStream = i;
			cout << i << "音频信息" << endl;
			cout << "sample_rate = " << as->codecpar->sample_rate << endl; //样本率
			//AVSampleFormat;  样本格式
			cout << "channels = " << as->codecpar->channels << endl;  //通道数
			//一帧数据  单通道样本数
			cout << "frame_size = " << as->codecpar->frame_size << endl;
			//1024*2（通道数）*2（16位2字节）=4096（一帧数据大小）   fps = sample_rate/frame_size

		}
		//视频 AVMEDIA_TYPE_VIDEO
		else if (as->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			videoStream = i;
			cout << i << "视频信息" << endl;
			cout << "width = " << as->codecpar->width << endl;
			cout << "height = " << as->codecpar->height << endl;
			//帧率 fps  分数转换  音频不一定有fps，且与视频不是一一对应的
			cout << "video fps = " << r2d(as->avg_frame_rate) << endl;
		}
	}

	//获取视频流（函数获取）
	videoStream = av_find_best_stream(ic, AVMEDIA_TYPE_VIDEO, -1, -1, NULL,0);
	//ic->streams[videoStream];

	//malloc AVPacket并初始化
	AVPacket *pkt = av_packet_alloc();
	for (;;) //使用无限循环读取视频帧数据
	{
		int re = av_read_frame(ic, pkt);
		if (re != 0)
		{
			//循环播放 每次到结尾，转到ms位置
			cout << "----------------------end----------------------" << endl;
			int ms = 3000;//三秒位置 根据时间基数（分数）转换
			//int ms = 120000;
			long long pos = (double)ms / (double)1000 * r2d(ic->streams[pkt->stream_index]->time_base);
			av_seek_frame(ic, videoStream, pos, AVSEEK_FLAG_BACKWARD | AVSEEK_FLAG_FRAME); //往后（早）找且找关键帧
			continue;
		}
		cout << "pkt->size = " << pkt->size << endl;
		//显示的时间
		cout << "pkt->pts = " << pkt->pts << endl;
		
		//转换为毫秒，方便做同步  音频视频的time_base不一样
		cout << "pkt->pts ms = " << pkt->pts * (r2d(ic->streams[pkt->stream_index]->time_base) * 1000) << endl;



		//解码时间  如果有闭帧，必须在前一帧和后一帧都解码完毕后再显示。若显示时间为2，则解码时间为3
		cout << "pkt->dts = " << pkt->dts << endl;
		if (pkt->stream_index == videoStream)
		{
			cout << "图像" << endl;
		}
		if (pkt->stream_index == audioStream)
		{
			cout << "音频" << endl;
		}

		//释放，引用计数-1  为0释放空间
		av_packet_unref(pkt);

		XSleep(200);
	}
	av_packet_free(&pkt);

	if (ic)
	{
		//释放封装上下文，并且把ic置0
		avformat_close_input(&ic);
	}

	getchar();
	return 0;
}