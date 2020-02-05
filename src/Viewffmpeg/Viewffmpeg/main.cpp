#include <iostream>
using namespace std;
//要引用C语言函数
extern "C" {
	#include <libavcodec/avcodec.h>
}
//预处理指令导入库
#pragma comment(lib,"avcodec.lib")

int main(int argc, char* argv[])
{
	//显示ffmpeg的编译配置
	cout << "Test FFmpeg.club" << endl;
#ifdef _WIN32 //32位和64位 win
    #ifdef _WIN64 //64位 win
        cout << "Windows X64" << endl;
    #else //32位win  宏定义为WIN32
        cout << "Windows X86" << endl;
    #endif
#else
	cout << "Linux" << endl;
#endif
	cout << avcodec_configuration() << endl;
	getchar();
	return 0;
}
