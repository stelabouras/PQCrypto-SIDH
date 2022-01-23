package wdi.androidsidhtest

import android.os.Bundle
import android.util.Log
import android.view.View
import android.widget.CheckBox
import android.widget.ScrollView
import androidx.appcompat.app.AppCompatActivity
import androidx.lifecycle.Observer
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import sidhjava.SidhNative
import sidhjava.test.SidhNativeTests
import wdi.androidsidhtest.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity() {

    @Suppress("unused")
    private val tag = this::class.java.simpleName

    private var runEverything = false
    private val launchScope = CoroutineScope(Dispatchers.Default)

    private lateinit var binding: ActivityMainBinding
    private var runTestsFor = arrayOf(0, 0, 0, 0)

    private var testsOnly = false
    private var arithmeticBench = false
    private var ecIsoBench = false
    private var diffieHellman = false

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityMainBinding.inflate(layoutInflater)

        setContentView(binding.root)

        binding.everythingSelect.setOnClickListener {
            if (it is CheckBox) {
                runEverything = it.isChecked
                switchSelectorVisibility(!runEverything)
                if (runEverything) selectEverything() else resetSelections()
            }
        }

        binding.p434select.setOnClickListener {
            if (it is CheckBox) {
                runTestsFor[P434] = if (it.isChecked) SidhNative.P434 else 0
            }
        }
        binding.p503select.setOnClickListener {
            if (it is CheckBox) {
                runTestsFor[P503] = if (it.isChecked) SidhNative.P503 else 0
            }
        }
        binding.p610select.setOnClickListener {
            if (it is CheckBox) {
                runTestsFor[P610] = if (it.isChecked) SidhNative.P610 else 0
            }
        }
        binding.p751select.setOnClickListener {
            if (it is CheckBox) {
                runTestsFor[P751] = if (it.isChecked) SidhNative.P751 else 0
            }
        }

        binding.testsOnlySelect.setOnClickListener {
            if (it is CheckBox) {
                testsOnly = it.isChecked
            }
        }
        binding.artihBenchSelect.setOnClickListener {
            if (it is CheckBox) {
                arithmeticBench = it.isChecked
            }
        }
        binding.ecIsoBenchSelect.setOnClickListener {
            if (it is CheckBox) {
                ecIsoBench = it.isChecked
            }
        }
        binding.diffieHelmanSelect.setOnClickListener {
            if (it is CheckBox) {
                diffieHellman = it.isChecked
            }
        }

        SidhCallback.logData.observe(this, Observer<Boolean> {
            val textBuffer = StringBuffer()
            for (txt in SidhCallback.dataList)  {
                textBuffer.append(txt)
            }
            binding.logData.text = textBuffer.toString()
            binding.scrollText.post { binding.scrollText.fullScroll(ScrollView.FOCUS_DOWN) }
        })
        SidhCallback.enableSidhLogging()

        binding.runButton.setOnClickListener {
            it.visibility = View.INVISIBLE
            runTests()
        }
    }

    override fun onDestroy() {
        super.onDestroy()
        binding.logData.text = ""
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
            binding.runButton.visibility = View.VISIBLE
            return
        }

        binding.logData.text = ""
        SidhCallback.dataList.clear()

        val startTime = System.nanoTime()
        launchScope.launch{
            for (runTest in runTestsFor) {
                if (runTest == 0) continue
                SidhNativeTests.runSidhTests(runTest, testMask)
            }
            // Four benchmark loops, each loop has 100 iterations.
            // It's just a rough crosscheck to see if the internal reporting makes
            // sense.
            val runTime = (System.nanoTime() - startTime) / 100
            SidhCallback.loggingCallback(2, "\nElapsed time (nano time): $runTime\n")
            CoroutineScope(Dispatchers.Main).launch {
                resetSelections()
           }
        }
    }

    private fun resetSelections() {
        binding.runButton.visibility = View.VISIBLE
        binding.p434select.isChecked = false
        binding.p503select.isChecked = false
        binding.p610select.isChecked = false
        binding.p751select.isChecked = false
        binding.testsOnlySelect.isChecked = false
        binding.artihBenchSelect.isChecked = false
        binding.ecIsoBenchSelect.isChecked = false
        binding.diffieHelmanSelect.isChecked = false

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
        if (show) binding.selectorGroup.visibility = View.VISIBLE else binding.selectorGroup.visibility = View.GONE
        binding.selectorGroup.requestLayout()
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
