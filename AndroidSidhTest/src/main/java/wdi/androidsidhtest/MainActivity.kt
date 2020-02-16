package wdi.androidsidhtest

import android.os.Bundle
import android.util.Log
import android.view.View
import android.widget.CheckBox
import android.widget.ScrollView
import androidx.appcompat.app.AppCompatActivity
import androidx.lifecycle.Observer
import kotlinx.android.synthetic.main.activity_main.*
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import sidhjava.SidhNative
import sidhjava.test.SidhNativeTests

class MainActivity : AppCompatActivity() {

    @Suppress("unused")
    private val tag = this::class.java.simpleName

    private var runEverything = false
    private val launchScope = CoroutineScope(Dispatchers.Default)

    private var runTestsFor = arrayOf(0, 0, 0, 0)

    private var testsOnly = false
    private var arithmeticBench = false
    private var ecIsoBench = false
    private var diffieHellman = false

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        everythingSelect.setOnClickListener {
            if (it is CheckBox) {
                runEverything = it.isChecked
                switchSelectorVisibility(!runEverything)
                if (runEverything) selectEverything() else resetSelections()
            }
        }

        p434select.setOnClickListener {
            if (it is CheckBox) {
                runTestsFor[P434] = if (it.isChecked) SidhNative.P434 else 0
            }
        }
        p503select.setOnClickListener {
            if (it is CheckBox) {
                runTestsFor[P503] = if (it.isChecked) SidhNative.P503 else 0
            }
        }
        p610select.setOnClickListener {
            if (it is CheckBox) {
                runTestsFor[P610] = if (it.isChecked) SidhNative.P610 else 0
            }
        }
        p751select.setOnClickListener {
            if (it is CheckBox) {
                runTestsFor[P751] = if (it.isChecked) SidhNative.P751 else 0
            }
        }

        testsOnlySelect.setOnClickListener {
            if (it is CheckBox) {
                testsOnly = it.isChecked
            }
        }
        artihBenchSelect.setOnClickListener {
            if (it is CheckBox) {
                arithmeticBench = it.isChecked
            }
        }
        ecIsoBenchSelect.setOnClickListener {
            if (it is CheckBox) {
                ecIsoBench = it.isChecked
            }
        }
        diffieHelmanSelect.setOnClickListener {
            if (it is CheckBox) {
                diffieHellman = it.isChecked
            }
        }

        SidhCallback.logData.observe(this, Observer<Boolean> {
            val textBuffer = StringBuffer()
            for (txt in SidhCallback.dataList)  {
                textBuffer.append(txt)
            }
            logData.text = textBuffer.toString()
            scrollText.post { scrollText.fullScroll(ScrollView.FOCUS_DOWN) }
        })
        SidhCallback.enableSidhLogging()

        runButton.setOnClickListener {
            it.visibility = View.INVISIBLE
            runTests()
        }
    }

    override fun onDestroy() {
        super.onDestroy()
        logData.text = ""
        SidhCallback.dataList.clear()
        SidhNative.disableLoggingCallback()
    }

    private fun runTests() {
        var testMask = 0
        if (testsOnly) {
            testMask = testMask or SidhNativeTests.TestsOnly
        }
        if (arithmeticBench) {
            testMask = testMask or SidhNativeTests.TestBenchmarks
        }
        if (ecIsoBench) {
            testMask = testMask or SidhNativeTests.EcisogFunctions
        }
        if (diffieHellman) {
            testMask = testMask or SidhNativeTests.DiffieHellman
        }

        if (testMask == 0) {
            runButton.visibility = View.VISIBLE
            return
        }

        logData.text = ""
        SidhCallback.dataList.clear()

        launchScope.launch{
            for (runTest in runTestsFor) {
                if (runTest == 0) continue
                SidhNativeTests.runSidhTests(runTest, testMask)
            }
            CoroutineScope(Dispatchers.Main).launch {
                resetSelections()
           }
        }
    }

    private fun resetSelections() {
        runButton.visibility = View.VISIBLE
        p434select.isChecked = false
        p503select.isChecked = false
        p610select.isChecked = false
        p751select.isChecked = false
        testsOnlySelect.isChecked = false
        artihBenchSelect.isChecked = false
        ecIsoBenchSelect.isChecked = false
        diffieHelmanSelect.isChecked = false

        testsOnly = false
        arithmeticBench = false
        ecIsoBench = false
        diffieHellman = false

        for (i in runTestsFor.indices) {
            runTestsFor[i] = 0
        }
    }

    private fun selectEverything() {
        runTestsFor[P434] = SidhNative.P434
        runTestsFor[P503] = SidhNative.P503
        runTestsFor[P610] = SidhNative.P610
        runTestsFor[P751] = SidhNative.P751

        testsOnly = true
        arithmeticBench = true
        ecIsoBench = true
        diffieHellman = true
    }

    private fun switchSelectorVisibility(show: Boolean) {
        if (show) selectorGroup.visibility = View.VISIBLE else selectorGroup.visibility = View.INVISIBLE
        selectorGroup.requestLayout()
    }

    companion object {
        init {
            Log.d ("MainActivity","loading Libs" )
            System.loadLibrary("c++_shared")
            System.loadLibrary("androidSidh")
            Log.d("MainActivity","done loading Libs")
        }

        // Indices of the run array
        private const val P434 = 0
        private const val P503 = 1
        private const val P610 = 2
        private const val P751 = 3
    }
}
