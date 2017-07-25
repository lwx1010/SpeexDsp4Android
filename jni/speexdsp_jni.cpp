#include <jni.h>
#include <string.h>
#include <unistd.h>
#include <android/log.h>
#include <speex/speex_preprocess.h>
#pragma comment(lib,"libspeexdsp.lib")

// log标签
#define TAG "Unity"
// 定义info信息
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG,__VA_ARGS__)
// 定义debug信息
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)
// 定义error信息
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG,__VA_ARGS__)

SpeexPreprocessState *m_st;
static int preprocess_frame_size;
static int preprocess_sampling_rate;

extern "C"
{
    JNIEXPORT void JNICALL Java_com_audio_SpeexDSP_initpreprocess(JNIEnv *env, jobject obj, jint frame_size, jint sampling_rate, jfloat agc_level)
    {
        preprocess_frame_size = frame_size;
        preprocess_sampling_rate = sampling_rate;
        
        m_st = speex_preprocess_state_init(preprocess_frame_size, preprocess_sampling_rate);
        int denoise = 1;
        int noiseSuppress = -25;
        speex_preprocess_ctl(m_st, SPEEX_PREPROCESS_SET_DENOISE, &denoise);
        speex_preprocess_ctl(m_st, SPEEX_PREPROCESS_SET_NOISE_SUPPRESS, &noiseSuppress);
        
        int agc = 1;
        speex_preprocess_ctl(m_st, SPEEX_PREPROCESS_SET_AGC, &agc);
        speex_preprocess_ctl(m_st, SPEEX_PREPROCESS_SET_AGC_LEVEL, &agc_level);
    }
    
    JNIEXPORT jshortArray JNICALL Java_com_audio_SpeexDSP_preprocess(JNIEnv *env, jobject obj, jshortArray lin, jint lin_size, jshortArray out)
    {
        short* buffer = new short[preprocess_frame_size];
        short* output_buffer = new short[lin_size];
        int nsamples = (lin_size - 1) / preprocess_frame_size + 1;

        int p = 0;
        short* linTemp;
        linTemp = env->GetShortArrayElements(lin, NULL);
        for (int i = 0; i < nsamples; i++)
        {
            int k = 0;
            memset(buffer, 0, preprocess_frame_size * sizeof(short));
            for (int j = i * preprocess_frame_size; j < i * preprocess_frame_size + preprocess_frame_size; j++)
            {
                buffer[k++] = linTemp[j];
            }
            spx_int16_t* ptr = (spx_int16_t*)buffer;
            speex_preprocess_run(m_st, ptr);
            for (int n = 0; n < preprocess_frame_size; n++)
            {
                output_buffer[p++] = buffer[n];
            }
        }

        env->SetShortArrayRegion(out, 0, lin_size, output_buffer);

        delete buffer;
        delete output_buffer;

        return out;
    }
    
    JNIEXPORT void JNICALL Java_com_audio_SpeexDSP_closepreprocess(JNIEnv *env, jobject obj)
    {
        speex_preprocess_state_destroy(m_st);
    }

}