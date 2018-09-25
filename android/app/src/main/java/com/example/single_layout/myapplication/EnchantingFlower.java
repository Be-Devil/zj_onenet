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

public class EnchantingFlower extends AppCompatActivity implements View.OnClickListener {

    TextView state;
    String state_s = "";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.enchantingflower_activity);
        MainActivity.Url = "api.heclouds.com/cmds?device_id=35138767";

        Button Flower_start = (Button) findViewById(R.id.Flower_start);
        Flower_start.setOnClickListener(this);
        Button Flower_closed = (Button) findViewById(R.id.Flower_closed);
        Flower_closed.setOnClickListener(this);
        Button Flower_light_start = (Button) findViewById(R.id.Flower_light_start);
        Flower_light_start.setOnClickListener(this);
        Button Flower_light_close = (Button) findViewById(R.id.Flower_light_close);
        Flower_light_close.setOnClickListener(this);
        Button Flower_music_start = (Button) findViewById(R.id.Flower_music_start);
        Flower_music_start.setOnClickListener(this);
        Button Flower_music_close = (Button) findViewById(R.id.Flower_music_close);
        Flower_music_close.setOnClickListener(this);

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

    public void onClick(View view) {
        if (MainActivity.mySocketServer.flag == false)
        {
            state_s = "未连接";
            phHandler.sendEmptyMessage(1);
        } else
            switch (view.getId()) {
                case R.id.Flower_start:
                    MainActivity.mySocketServer.send_b(new byte[]{(byte) 0xaa, (byte) 0x03, (byte) 0x03, (byte) 0x01, (byte) 0x2a});
                    state_s = "妖艳花已开启（5秒后自动关闭）";
                    phHandler.sendEmptyMessage(1);
                    break;

                case R.id.Flower_closed:
                    MainActivity.mySocketServer.send_b(new byte[]{(byte) 0xaa, (byte) 0x03, (byte) 0x03, (byte) 0x02, (byte) 0x2a});
                    state_s = "妖艳花已关闭";
                    phHandler.sendEmptyMessage(1);
                    break;
                case R.id.Flower_light_start:
                    MainActivity.mySocketServer.send_b(new byte[]{(byte) 0xaa, (byte) 0x03, (byte) 0x03, (byte) 0x05, (byte) 0x2a});
                    state_s = "妖艳花灯光已开启";
                    phHandler.sendEmptyMessage(1);
                    break;

                case R.id.Flower_light_close:
                    MainActivity.mySocketServer.send_b(new byte[]{(byte) 0xaa, (byte) 0x03, (byte) 0x03, (byte) 0x06, (byte) 0x2a});
                    state_s = "妖艳花灯光已关闭";
                    phHandler.sendEmptyMessage(1);
                    break;

                case R.id.Flower_music_start:
                    MainActivity.mySocketServer.send_b(new byte[]{(byte) 0xaa, (byte) 0x03, (byte) 0x03, (byte) 0x03, (byte) 0x2a});
                    state_s = "妖艳花音乐已开启";
                    phHandler.sendEmptyMessage(1);
                    break;

                case R.id.Flower_music_close:
                    MainActivity.mySocketServer.send_b(new byte[]{(byte) 0xaa, (byte) 0x03, (byte) 0x03, (byte) 0x04, (byte) 0x2a});
                    state_s = "妖艳花音乐已关闭";
                    phHandler.sendEmptyMessage(1);
                    break;
            }

        switch (view.getId()) {
            case R.id.Return:
                Intent intent = new Intent(EnchantingFlower.this, MainActivity.class);
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
