package com.example.administrator.myapplication;

import android.annotation.SuppressLint;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;
import android.view.View;

import java.io.IOException;
import java.io.UnsupportedEncodingException;

import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import com.example.single_layout.myapplication.*;

import static com.example.administrator.myapplication.HttpUtils.post1;
import static com.example.administrator.myapplication.HttpUtils.post2;
import static com.example.administrator.myapplication.HttpUtils.post3;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {

    public Intent intent;

    public static boolean init = true;
    public static int Pattern = 0;
    public static MySocketServer mySocketServer;
    public static String massage = "";
    public static boolean send_massage = false;
    public static boolean connet_onenet = false;
    public static String Url = "";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.content_main);

        if(init == true) {
            WebConfig webConfig = new WebConfig();
            webConfig.setPort(1811);
            webConfig.setMaxParallels(10);
            mySocketServer = new MySocketServer(webConfig);
            mySocketServer.startServerAsync();
            init = false;
        }

        Thread1.start();

        Button Pattern = (Button) findViewById(R.id.Pattern);
        Pattern.setOnClickListener(this);
        Button EnchantingFlower = (Button) findViewById(R.id.EnchantingFlower);
        EnchantingFlower.setOnClickListener(this);
        Button SmartCar = (Button) findViewById(R.id.SmartCar);
        SmartCar.setOnClickListener(this);
        Button WaterSprayTank = (Button) findViewById(R.id.WaterSprayTank);
        WaterSprayTank.setOnClickListener(this);
        Button InfraredLaser = (Button) findViewById(R.id.InfraredLaser);
        InfraredLaser.setOnClickListener(this);
        Button ToyCar = (Button) findViewById(R.id.ToyCar);
        ToyCar.setOnClickListener(this);
        Button sack = (Button) findViewById(R.id.sack);
        sack.setOnClickListener(this);
        Button snake = (Button) findViewById(R.id.snake);
        snake.setOnClickListener(this);
        Button mouse = (Button) findViewById(R.id.mouse);
        mouse.setOnClickListener(this);
        Button FMoppingRobot = (Button) findViewById(R.id.FMoppingRobot);
        FMoppingRobot.setOnClickListener(this);
        Button SteeringEngine = (Button) findViewById(R.id.SteeringEngine);
        SteeringEngine.setOnClickListener(this);
        Button text = (Button) findViewById(R.id.text);
        text.setOnClickListener(this);
        Button text2 = (Button) findViewById(R.id.text2);
        text2.setOnClickListener(this);

    }

    protected void onDestroy() {

        try {
            if (mySocketServer.socket != null && !mySocketServer.socket.isClosed()) {
                mySocketServer.socket.close();                                                       ///关闭套接字
                mySocketServer.bInputStream.close();                                                  ///关闭输入流
                mySocketServer.bOutputStream.close();                                                 ///关闭输出流
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        super.onDestroy();
    }

    /****************************************/
    /*
    Button控制
     */
    /****************************************/

    public void onClick(View view) {
        switch (view.getId()) {
            case R.id.Pattern:
                intent = new Intent(MainActivity.this, init_connet.class);
                startActivity(intent);
//                    if (Pattern++ % 2 == 0) {
//                        弹幕("Internet模式启动");
//                        mySocketServer.stopServerAsync();
//                    } else {
//                        弹幕("局域网模式启动");
//                        mySocketServer.startServerAsync();
//                    }
                break;
            case R.id.EnchantingFlower:
                intent = new Intent(MainActivity.this, EnchantingFlower.class);
                startActivity(intent);
                break;
            case R.id.SmartCar:
                intent = new Intent(MainActivity.this, SmartCar.class);
                startActivity(intent);
                break;
            case R.id.WaterSprayTank:
                intent = new Intent(MainActivity.this, WaterSprayTank.class);
                startActivity(intent);
                break;
            case R.id.InfraredLaser:
                intent = new Intent(MainActivity.this, InfraredLaser.class);
                startActivity(intent);
                break;
            case R.id.ToyCar:
                intent = new Intent(MainActivity.this, ToyCar.class);
                startActivity(intent);
                break;
            case R.id.sack:
                intent = new Intent(MainActivity.this, sack.class);
                startActivity(intent);
                break;
            case R.id.snake:
                intent = new Intent(MainActivity.this, snake.class);
                startActivity(intent);
                break;
            case R.id.mouse:
                intent = new Intent(MainActivity.this, mouse.class);
                startActivity(intent);
                break;
            case R.id.FMoppingRobot:
                intent = new Intent(MainActivity.this, FMoppingRobot.class);
                startActivity(intent);
                break;
            case R.id.SteeringEngine:
                intent = new Intent(MainActivity.this, SteeringEngine.class);
                startActivity(intent);
                break;
            case R.id.text:
                post1("http://api.heclouds.com/cmds?device_id=39151104", "AA 03 03 00 2A");
                break;
            case R.id.text2:
                post1("http://api.heclouds.com/cmds?device_id=39151104", "");
                break;

        }


    }

    /****************************************/
    /*
    other
     */
    /****************************************/
    public static void delay(int time) {
        try {
            Thread.sleep(time);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    Thread Thread1 = new Thread(new Runnable() {
        @Override
        public void run() {
            while (true) {
                if( send_massage == true) {
                    send_massage = false;
                    phHandler1.sendEmptyMessage(3);
                }

            }
        }

    });

    @SuppressLint("HandlerLeak")
    public Handler phHandler1 = new Handler() {
        @SuppressLint("WrongViewCast")
        public void handleMessage(Message msg) {
            if (msg.what == 3) {
                弹幕(massage);
            }
        }

        ;
    };

    @SuppressLint("WrongConstant")
    public void 弹幕(String x) {
        Toast.makeText(MainActivity.this, x, 500).show();
    }


}
