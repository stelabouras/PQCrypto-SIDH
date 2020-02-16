package wdi.androidsidhtest

import android.util.Log
import androidx.lifecycle.MutableLiveData
import sidhjava.SidhNative
import java.util.concurrent.ConcurrentLinkedQueue

object SidhCallback: SidhNative() {

    val logData = MutableLiveData<Boolean>()

    val dataList = ConcurrentLinkedQueue<String>()

    override fun loggingCallback(level: Int, data: String?) {
        data?.let {
            dataList.add(data)
            logData.postValue(true)
//            Log.d("SidhCallback", "$data")
        }?: Log.w("SidhCallback", "Empty data")
    }

    fun enableSidhLogging() {
        val result = enableLoggingCallback()
        Log.d("SidhCallback", "Enable result $result")
    }
}