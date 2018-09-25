package com.example.single_layout.myapplication;

import android.annotation.SuppressLint;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.net.wifi.WifiConfiguration;
import android.net.wifi.WifiManager;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;
import android.view.Menu;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.AutoCompleteTextView;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import com.example.administrator.myapplication.MainActivity;
import com.example.administrator.myapplication.R;
import com.tencent.rtmp.ITXLivePlayListener;
import com.tencent.rtmp.TXLiveConstants;
import com.tencent.rtmp.TXLivePlayConfig;
import com.tencent.rtmp.TXLivePlayer;
import com.tencent.rtmp.ui.TXCloudVideoView;

import java.io.UnsupportedEncodingException;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

import static android.net.wifi.WifiManager.WIFI_STATE_DISABLED;
import static com.example.administrator.myapplication.HttpUtils.post1;

public class init_connet extends AppCompatActivity implements View.OnClickListener {


    private EditText myEditText;
    private static final String TEMP_INFO = "temp_info";
    WifiManager wifiManager;
    private boolean flag=false;

    private AutoCompleteTextView WiFi_name;
    private AutoCompleteTextView WiFi_password;
    private AutoCompleteTextView server_ip;
    private AutoCompleteTextView server_port;

    Button open_Wifi;
    Button open_WifiAp;
    @SuppressLint("WifiManagerLeak")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.init_connet);
        MainActivity.Url = "api.heclouds.com/cmds?device_id=39151104";
        wifiManager = (WifiManager) getSystemService(Context.WIFI_SERVICE);

        Button in_connet = (Button) findViewById(R.id.in_connet);
        in_connet.setOnClickListener(this);
        Button Return = (Button) findViewById(R.id.Return);
        Return.setOnClickListener(this);
        open_Wifi = (Button) findViewById(R.id.open_Wifi);
        open_Wifi.setOnClickListener(this);
        open_WifiAp = (Button) findViewById(R.id.open_WifiAp);
        open_WifiAp.setOnClickListener(this);

        String[] WiFi_name_s = new String[]{"HUAWEI","TP-LINK_D2D2","ChinaNet-fk"};
        String[] WiFi_password_s = new String[]{"zjwy1234","zxc123456789","1l3o1v4e5y2o0u"};
        String[] server_ip_s = new String[]{"192.168.43.1","183.230.40.40","192.168.168.101"};
        String[] server_port_s = new String[]{"1811"};

        ArrayAdapter<String> WiFi_name_a = new ArrayAdapter<String>(this,
                android.R.layout.simple_dropdown_item_1line, WiFi_name_s);
        ArrayAdapter<String> WiFi_password_a = new ArrayAdapter<String>(this,
                android.R.layout.simple_dropdown_item_1line, WiFi_password_s);
        ArrayAdapter<String> server_ip_a = new ArrayAdapter<String>(this,
                android.R.layout.simple_dropdown_item_1line, server_ip_s);
        ArrayAdapter<String> server_port_a = new ArrayAdapter<String>(this,
                android.R.layout.simple_dropdown_item_1line, server_port_s);

        WiFi_name = (AutoCompleteTextView) findViewById(R.id.WiFi_name);
        WiFi_password = (AutoCompleteTextView) findViewById(R.id.WiFi_password);
        server_ip = (AutoCompleteTextView) findViewById(R.id.server_ip);
        server_port = (AutoCompleteTextView) findViewById(R.id.server_port);

        WiFi_name.setAdapter(WiFi_name_a);     // 绑定adapter
        WiFi_password.setAdapter(WiFi_password_a);     // 绑定adapter
        server_ip.setAdapter(server_ip_a);     // 绑定adapter
        server_port.setAdapter(server_port_a);     // 绑定adapter

        WiFi_name.setThreshold(0);
        WiFi_password.setThreshold(0);
        server_ip.setThreshold(0);
        server_port.setThreshold(0);

        WiFi_name.setOnFocusChangeListener(new View.OnFocusChangeListener() {
            @Override
            public void onFocusChange(View v, boolean hasFocus) {
                AutoCompleteTextView view = (AutoCompleteTextView) v;
                if (hasFocus) {
                    view.showDropDown();
                }
            }
        });

        WiFi_password.setOnFocusChangeListener(new View.OnFocusChangeListener() {
            @Override
            public void onFocusChange(View v, boolean hasFocus) {
                AutoCompleteTextView view = (AutoCompleteTextView) v;
                if (hasFocus) {
                    view.showDropDown();
                }
            }
        });

        server_ip.setOnFocusChangeListener(new View.OnFocusChangeListener() {
            @Override
            public void onFocusChange(View v, boolean hasFocus) {
                AutoCompleteTextView view = (AutoCompleteTextView) v;
                if (hasFocus) {
                    view.showDropDown();
                }
            }
        });

        server_port.setOnFocusChangeListener(new View.OnFocusChangeListener() {
            @Override
            public void onFocusChange(View v, boolean hasFocus) {
                AutoCompleteTextView view = (AutoCompleteTextView) v;
                if (hasFocus) {
                    view.showDropDown();
                }
            }
        });

        if(isApOn()) {
            MainActivity.massage = "热点已开启，Wifi已关闭";
            MainActivity.send_massage = true;
            open_Wifi.setText("Wifi已关闭");
            open_WifiAp.setText("热点已开启");
        }else if(wifiManager.getWifiState() != WIFI_STATE_DISABLED){
            MainActivity.massage = "热点已关闭,Wifi已开启";
            MainActivity.send_massage = true;
            open_Wifi.setText("Wifi已开启");
            open_WifiAp.setText("热点已关闭");
        }else{
            MainActivity.massage = "热点已关闭,Wifi已关闭";
            MainActivity.send_massage = true;
            open_Wifi.setText("Wifi已关闭");
            open_WifiAp.setText("热点已关闭");

        }

    }
//
//    /**
//     * 初始化AutoCompleteTextView，最多显示5项提示，使
//     * AutoCompleteTextView在一开始获得焦点时自动提示
//     *
//     * @param field 保存在sharedPreference中的字段名
//     * @param auto  要操作的AutoCompleteTextView
//     */
//    private void initAutoComplete(String field, AutoCompleteTextView auto) {
//        SharedPreferences sp = getSharedPreferences("network_url", 0);
//        String longhistory = sp.getString("history", "1811");
//        String[] hisArrays = longhistory.split(",");
//        ArrayAdapter<String> adapter = new ArrayAdapter<String>(this,
//                android.R.layout.simple_dropdown_item_1line, hisArrays);
//        //只保留最近的50条的记录
//        if (hisArrays.length > 50) {
//            String[] newArrays = new String[50];
//            System.arraycopy(hisArrays, 0, newArrays, 0, 50);
//            adapter = new ArrayAdapter<String>(this,
//                    android.R.layout.simple_dropdown_item_1line, newArrays);
//        }
//        auto.setAdapter(adapter);
//        auto.setDropDownHeight(350);
//        auto.setThreshold(1);
//        auto.setCompletionHint("最近的5条记录");
//        auto.setOnFocusChangeListener(new View.OnFocusChangeListener() {
//            @Override
//            public void onFocusChange(View v, boolean hasFocus) {
//                AutoCompleteTextView view = (AutoCompleteTextView) v;
//                if (hasFocus) {
//                    view.showDropDown();
//                }
//            }
//        });
//    }
//
//
//    /**
//     * 把指定AutoCompleteTextView中内容保存到sharedPreference中指定的字符段
//     *
//     * @param field 保存在sharedPreference中的字段名
//     * @param auto  要操作的AutoCompleteTextView
//     */
//    private void saveHistory(String field, AutoCompleteTextView auto) {
//        String text = auto.getText().toString();
//        SharedPreferences sp = getSharedPreferences("network_url", 0);
//        String longhistory = sp.getString("history", field);
//        if (!longhistory.contains(text + ",")) {
//            StringBuilder sb = new StringBuilder(longhistory);
//            sb.insert(0, text + ",");
//            sp.edit().putString("history", sb.toString()).commit();
//        }
//
//    }
        /****************************************/
    /*
    Button控制
     */
        /****************************************/

        public void onClick (View view){

            switch (view.getId()) {
                case R.id.open_Wifi:
                        if(wifiManager.getWifiState() == WIFI_STATE_DISABLED) {
                            wifiManager.setWifiEnabled(true);
                            MainActivity.massage = "Wifi开启，热点关闭";
                            MainActivity.send_massage = true;
                            phHandler.sendEmptyMessage(3);
                            phHandler.sendEmptyMessage(2);
                        }
                        else {
                            wifiManager.setWifiEnabled(false);
                            MainActivity.massage = "Wifi关闭";
                            MainActivity.send_massage = true;
                            phHandler.sendEmptyMessage(4);
                        }
                    break;
                case R.id.open_WifiAp:
                    //如果是打开状态就关闭，如果是关闭就打开
                    setWifiApEnabled(!isApOn());
                    break;
                case R.id.in_connet:
                    MainActivity.mySocketServer.send_b(Text((byte) 0X01, WiFi_name.getText().toString()));
                    MainActivity.mySocketServer.send_b(Text((byte) 0X02, WiFi_password.getText().toString()));
                    MainActivity.mySocketServer.send_b(Text((byte) 0X03, server_ip.getText().toString()));
                    MainActivity.mySocketServer.send_b(Text((byte) 0X04, server_port.getText().toString()));
                    MainActivity.mySocketServer.send_b(new byte[]{(byte) 0xaa, (byte) 0x03, (byte) 0x01, (byte) 0x05, (byte) 0x2a});

                    if (server_ip.getText().toString().equals("183.230.40.40")) {
                        MainActivity.Pattern = 1;
                        MainActivity.connet_onenet = true;
                        MainActivity.massage = "设备接入onenet服务器，请接入Intenet";
                        MainActivity.send_massage = true;
//                        MainActivity.connet_onenet = true;
                    } else {
                        MainActivity.connet_onenet = false;
                        MainActivity.Pattern = 0;
                    }
                    break;
                case R.id.Return:
                    Intent intent = new Intent(init_connet.this, MainActivity.class);
                    startActivity(intent);
                    break;
            }
        }

    public boolean isApOn() {
        try {
            Method method = wifiManager.getClass().getDeclaredMethod("isWifiApEnabled");
            method.setAccessible(true);
            return (Boolean) method.invoke(wifiManager);
        }
        catch (Throwable ignored) {}
        return false;
    }

    // wifi热点开关
    public boolean setWifiApEnabled(boolean enabled) {
        if(enabled) {
            MainActivity.massage = "热点开启,Wifi关闭";
            MainActivity.send_massage = true;
            phHandler.sendEmptyMessage(1);
            phHandler.sendEmptyMessage(4);
        }else{
            MainActivity.massage = "热点关闭";
            MainActivity.send_massage = true;
            phHandler.sendEmptyMessage(2);
        }

        if (enabled) { // disable WiFi in any case
            //wifi和热点不能同时打开，所以打开热点的时候需要关闭wifi
            wifiManager.setWifiEnabled(false);
        }
        try {
            //热点的配置类
            WifiConfiguration apConfig = new WifiConfiguration();
            //配置热点的名称(可以在名字后面加点随机数什么的)
            apConfig.SSID = "HUAWEI";
            //配置热点的密码
            apConfig.preSharedKey="zjwy1234";
            apConfig.allowedKeyManagement.set(4);
            //通过反射调用设置热点
            Method method = wifiManager.getClass().getMethod(
                    "setWifiApEnabled", WifiConfiguration.class, Boolean.TYPE);
            //返回热点打开状态
            return (Boolean) method.invoke(wifiManager, apConfig, enabled);
        } catch (Exception e) {
            return false;
        }
    }

        public byte[] bytesend ( byte b, byte[] sbyte){
            byte[] textbyte = new byte[sbyte.length + 5];
            textbyte[0] = (byte) 0xAA;
            textbyte[1] = (byte) (sbyte.length + 3);
            textbyte[2] = (byte) 0X01;
            textbyte[3] = b;
            for (int i = 0; i < sbyte.length; i++) {
                textbyte[i + 4] = sbyte[i];
            }
            textbyte[sbyte.length + 4] = 0X2A;
            return textbyte;
        }

        public byte[] Text ( byte b, String src){
            try {
                byte[] sbyte = bytesend(b, src.getBytes("GBK"));
                return sbyte;
            } catch (UnsupportedEncodingException e) {
                e.printStackTrace();
                return (new byte[]{});
            }


        }

    @SuppressLint("HandlerLeak")
    public Handler phHandler = new Handler() {
        @SuppressLint("WrongViewCast")
        public void handleMessage(Message msg) {
            if (msg.what == 1) {
                open_WifiAp.setText("热点已开启");
            }
            if (msg.what == 2) {
                open_WifiAp.setText("热点已关闭");
            }
            if (msg.what == 3) {
                open_Wifi.setText("Wifi已开启");
            }
            if (msg.what == 4) {
                open_Wifi.setText("Wifi已关闭");
            }
        }

        ;
    };
}
