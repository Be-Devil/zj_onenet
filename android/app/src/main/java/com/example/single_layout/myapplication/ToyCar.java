package com.example.single_layout.myapplication;

import android.annotation.SuppressLint;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.content.res.Configuration;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;

import com.example.administrator.myapplication.MainActivity;
import com.example.administrator.myapplication.R;
import com.tencent.rtmp.ITXLivePlayListener;
import com.tencent.rtmp.TXLiveConstants;
import com.tencent.rtmp.TXLivePlayConfig;
import com.tencent.rtmp.TXLivePlayer;
import com.tencent.rtmp.ui.TXCloudVideoView;

import static com.example.administrator.myapplication.HttpUtils.post1;

public class ToyCar extends AppCompatActivity implements View.OnClickListener {

    TextView state;
    String state_s = "";
    boolean longClicked = true;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.toycar_activity);
        MainActivity.Url = "api.heclouds.com/cmds?device_id=36770243";
        ImageView go = (ImageView) findViewById(R.id.go);
        go.setOnTouchListener(mImageViewTouchHandler);
        ImageView left = (ImageView) findViewById(R.id.left);
        left.setOnTouchListener(mImageViewTouchHandler);
        ImageView right = (ImageView) findViewById(R.id.right);
        right.setOnTouchListener(mImageViewTouchHandler);
        ImageView back = (ImageView) findViewById(R.id.back);
        back.setOnTouchListener(mImageViewTouchHandler);

        state = (TextView) findViewById(R.id.state);

        Button Return = (Button) findViewById(R.id.Return);
        Return.setOnClickListener(this);

        initVideoView();
    }

    /****************************************/
    /*
    Button控制
     */
    /****************************************/
    public ImageView.OnTouchListener mImageViewTouchHandler = new ImageView.OnTouchListener() {
        @Override
        public boolean onTouch(View v, MotionEvent event) {
            final int id = v.getId();
            if (event.getAction() == MotionEvent.ACTION_DOWN) {
                longClicked = true;
                new Thread(new Runnable() {
                    @Override
                    public void run() {
     //                   while (longClicked) {
                            if (MainActivity.Pattern % 2 == 0) {
                                if (MainActivity.mySocketServer.flag == false) {
                                    state_s = "未连接";
                                    phHandler.sendEmptyMessage(1);
                                } else
                                    switch (id) {
                                        case R.id.go:
                                            MainActivity.mySocketServer.send_b(new byte[]{(byte) 0xaa, (byte) 0x03, (byte) 0x06, (byte) 0x01, (byte) 0x2a});
                                            state_s = "前进";
                                            phHandler.sendEmptyMessage(1);
                                            break;
                                        case R.id.left:
                                            MainActivity.mySocketServer.send_b(new byte[]{(byte) 0xaa, (byte) 0x03, (byte) 0x06, (byte) 0x02, (byte) 0x2a});
                                            state_s = "左转";
                                            phHandler.sendEmptyMessage(1);
                                            break;
                                        case R.id.right:
                                            MainActivity.mySocketServer.send_b(new byte[]{(byte) 0xaa, (byte) 0x03, (byte) 0x06, (byte) 0x03, (byte) 0x2a});
                                            state_s = "右转";
                                            phHandler.sendEmptyMessage(1);
                                            break;
                                        case R.id.back:
                                            MainActivity.mySocketServer.send_b(new byte[]{(byte) 0xaa, (byte) 0x03, (byte) 0x06, (byte) 0x04, (byte) 0x2a});
                                            state_s = "后退";
                                            phHandler.sendEmptyMessage(1);
                                            break;
                                    }
                            } else {
                                switch (id) {
                                    case R.id.go:
                                        state_s = post1("http://api.heclouds.com/cmds?device_id=36770243", "1111");
                                        phHandler.sendEmptyMessage(1);
                                        break;
                                    case R.id.left:
                                        state_s = post1("http://api.heclouds.com/cmds?device_id=36770243", "2111");
                                        phHandler.sendEmptyMessage(1);
                                        break;
                                    case R.id.right:
                                        state_s = post1("http://api.heclouds.com/cmds?device_id=36770243", "3111");
                                        phHandler.sendEmptyMessage(1);
                                        break;
                                    case R.id.back:
                                        state_s = post1("http://api.heclouds.com/cmds?device_id=36770243", "4111");
                                        phHandler.sendEmptyMessage(1);
                                        break;
                                }

                            }

                    }
                }).start();
            } else if (event.getAction() == MotionEvent.ACTION_UP) {
                longClicked = false;

                if (MainActivity.Pattern % 2 == 0)
                    MainActivity.mySocketServer.send_b(new byte[]{(byte) 0xaa, (byte) 0x03, (byte) 0x06, (byte) 0x05, (byte) 0x2a});
                else
                    state_s = post1("http://api.heclouds.com/cmds?device_id=36770243", "5111");
                state_s = "停止";
                phHandler.sendEmptyMessage(1);

            }
            return true;
        }
    };


    public void onClick(View view) {
        switch (view.getId()) {
            case R.id.Return:
                Intent intent = new Intent(ToyCar.this, MainActivity.class);
                startActivity(intent);
                break;
        }

    }

    public void initVideoView() {
        TXCloudVideoView video_view = (TXCloudVideoView) findViewById(R.id.video_view);
        TXLivePlayer txLivePlayer;
        txLivePlayer = new TXLivePlayer(this);
        txLivePlayer.setPlayerView(video_view);
        TXLivePlayConfig txLivePlayConfig = new TXLivePlayConfig();
        txLivePlayConfig.setAutoAdjustCacheTime(true);
        txLivePlayConfig.setMinAutoAdjustCacheTime(1);
        txLivePlayConfig.setMaxAutoAdjustCacheTime(5);
        txLivePlayer.setConfig(txLivePlayConfig);
        txLivePlayer.enableHardwareDecode(true);
        ITXLivePlayListener itxLivePlayListener = new ITXLivePlayListener() {
            @Override
            public void onPlayEvent(int i, Bundle bundle) {
                if (i == TXLiveConstants.PLAY_EVT_RCV_FIRST_I_FRAME) {
                    //开始播放去掉loading菊花
                }
            }

            @Override
            public void onNetStatus(Bundle bundle) {

            }
        };
        txLivePlayer.setPlayListener(itxLivePlayListener);
        txLivePlayer.startPlay("rtmp://pili-live-rtmp-zj.realgamecloud.com/zengjjing/bangongshi", TXLivePlayer.PLAY_TYPE_LIVE_RTMP_ACC);

    }

    public Handler phHandler = new Handler() {
        @SuppressLint("WrongViewCast")
        public void handleMessage(Message msg) {
            if (msg.what == 1) {
                state.setText(state_s);
            }
        }
    };
}
