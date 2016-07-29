package com.example.wenfahu.lane_droid;

import android.app.Activity;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.SurfaceView;

import org.opencv.android.CameraBridgeViewBase;
import org.opencv.core.Mat;


public class MainActivity extends Activity implements CameraBridgeViewBase.CvCameraViewListener2  {
    private static final String TAG = "OCV-lane";
    private CameraBridgeViewBase mOpenCvCameraView;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mOpenCvCameraView = (CameraBridgeViewBase) findViewById(R.id.HelloOpenCvView);
        mOpenCvCameraView.setVisibility(SurfaceView.VISIBLE);
        mOpenCvCameraView.setMaxFrameSize(320, 240);
        mOpenCvCameraView.setCvCameraViewListener(this);
    }

    static {
        System.loadLibrary("native");
    }

    @Override
    public void onResume(){
        super.onResume();
        mOpenCvCameraView.enableView();
    }

    @Override
    public void onPause(){
        super.onPause();
        if (mOpenCvCameraView != null)
            mOpenCvCameraView.disableView();
    }

    @Override
    public void onDestroy(){
        super.onDestroy();
        if (mOpenCvCameraView != null)
            mOpenCvCameraView.disableView();
    }
    public void onCameraViewStarted(int width, int height) {
    }

    public void onCameraViewStopped() {
    }

    public Mat onCameraFrame(CameraBridgeViewBase.CvCameraViewFrame inputFrame) {
        Mat rgba = inputFrame.rgba();
        long t1 = System.currentTimeMillis();
        processOpt(rgba.getNativeObjAddr());
        long t2 = System.currentTimeMillis();
        Log.i(TAG, String.valueOf(t2-t1));
        return rgba;
    }

    public native void process(long frameAddr);
    public native void processOpt(long frameAddr);
}
