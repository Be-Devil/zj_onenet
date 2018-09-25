package com.example.administrator.myapplication;

import java.io.IOException;
import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.TimeUnit;


import okhttp3.Call;
import okhttp3.Callback;
import okhttp3.FormBody;

import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.Response;

public class HttpUtils {

    static String Text;

    /****************************************/
    /*
    Internet模式
     */

    /****************************************/
    public static String post2(final String url, final byte[] body) {
        new Thread(new Runnable() {
            @Override
            public void run() {
                HttpUtils httpUtils = new HttpUtils();

                Map<String, String> params = new HashMap<>();
                params.put("one", body + "*");
                httpUtils.postOkhttp(url, params, new Callback() {
                    @Override
                    public void onFailure(Call call, IOException e) {

                        System.out.println("Fail");
                    }

                    @Override
                    public void onResponse(Call call, Response response) throws IOException {
                        if (response.code() == 200) {
                            Text = "" + response.body().string();
                            System.out.println(Text);
                        }
                    }
                });
            }
        }).start();
        return Text;
    }
    public static String post1(final String url, final String body) {
        new Thread(new Runnable() {
            @Override
            public void run() {
                HttpUtils httpUtils = new HttpUtils();

                Map<String, String> params = new HashMap<>();
                params.put("one", body + "*");
                httpUtils.postOkhttp(url, params, new Callback() {
                    @Override
                    public void onFailure(Call call, IOException e) {

                        System.out.println("Fail");
                    }

                    @Override
                    public void onResponse(Call call, Response response) throws IOException {
                        if (response.code() == 200) {
                            Text = "" + response.body().string();
                            System.out.println(Text);
                        }
                    }
                });
            }
        }).start();
        return Text;
    }

    public static String post3(final String url, final byte[] body) {
        int i=0,num= 0,length = body.length;
        for(i=0;i<length;i++,num++)
            if((int)(body[i]&0xff)  >= 0x80)
                num=+2;
        final byte[] body1 = new byte[num];
        i=0;num= 0;
        while(length-- != 0){
            if((int)(body[i]&0xff)  >= 0x80) {
                body1[num] = 0;
                body1[num + 1] = (byte) ((int)(body[i]&0xff) / 16);
                body1[num + 2] = (byte) ((int)(body[i]&0xff) % 16);
                num = num + 3;
            }else {
                body1[num] = body[i];
                num++;
            }
            i++;
        }
        new Thread(new Runnable() {
            @Override
            public void run() {
                HttpUtils httpUtils = new HttpUtils();

                Map<String, String> params = new HashMap<>();
                params.put("one", new String(body1) + "*");
                httpUtils.postOkhttp(url, params, new Callback() {
                    @Override
                    public void onFailure(Call call, IOException e) {

                        System.out.println("Fail");
                    }

                    @Override
                    public void onResponse(Call call, Response response) throws IOException {
                        if (response.code() == 200) {
                            Text = "" + response.body().string();
                            System.out.println(Text);
                        }
                    }
                });
            }
        }).start();
        return Text;
    }

    public  void postOkhttp(String url, Map<String, String> map, Callback callBack) {
        OkHttpClient client=  client = new OkHttpClient
                .Builder()
                .connectTimeout(5, TimeUnit.SECONDS)
                .writeTimeout(10,TimeUnit.SECONDS)
                .readTimeout(10, TimeUnit.SECONDS)
                .build();
        //上传文字格式 数据的传输，区别于多媒体输出
        FormBody.Builder formbody = new FormBody.Builder();
        if (map != null && !map.isEmpty()) {
            //上传参数
            for (String key : map.keySet()) {
                formbody.add(key, map.get(key));
            }
            //创建请求体
            FormBody body = formbody.build();
            final Request request = new Request.Builder()
                    .url(url)//请求的url
                    .post(body)

                    .addHeader("api-key","aSvwTmvbhXUvWJw4mVVbyNkjv=M=")
                    .build();
            Call call = client.newCall(request);
            //异步请求方式
            call.enqueue(callBack);
        } else {
            //创建请求体
            FormBody body = formbody.build();
            Request request = new Request.Builder()
                    .url(url)
                    .post(body)
                    .build();
            Call call = client.newCall(request);
            //异步请求方式
            call.enqueue(callBack);
        }
    }

}
