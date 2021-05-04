# How to use
1. Start echo device.

```sh
cd virtual_driver
gcc echo.c
./a.out
```

2. Start main.c。

```sh
sh make_all.sh
./main.out
```

# Examples.

```C
//　各初期化関数
CCInitializer_init();
TCInitializer_init();
PCInitializer_init();
CSInitializer_init();

//　通信スレッド等の立ち上げ
TCLauncher_launchAll();

// デバイスIDとデバイス名前、レジスタ番号＆レジスタ型とレジスタ名を関連付ける
CSDataBase_setDevice("counter", 0x113);
CSDataBase_setRegister("counter", "count1", 0x00, CCVariable_data_Uint32);
CSDataBase_setRegister("counter", "count2", 0x04, CCVariable_data_Uint32);

// デバイス名とそのレジスタ名を指定し、FIFOで値を読み取れるようにする
CSRegSignal_userBind("counter", "count1");
TCSignal_t count1_sig = CSRegSignal_openUser("counter", "count1", "look");

// デバイス名とそのレジスタ名を指定し、共有変数として値を読み取れるようにする
CSRegShare_userBind("counter", "count2");
TCShare_t count2_shr = CSRegShare_openUser("counter", "count2");

for(size_t i = 0; i < 15; i++)
{
    // デバイス名とそのレジスタ名を指定し、デバイスに任意のデータをリクエストする。
    CSSender_request("counter", "count1");
    CSSender_request("counter", "count2");

    // CSRegSignal_recvは、新しいデータが来るまで待機する。
    CCLOG_INFO("%d", CSRegSignal_recv(count1_sig).Uint32);
    // CSRegShare_readは、直近で送られてきたデータを読み取る。
    CCLOG_INFO("%d", CSRegShare_read(count2_shr).Uint32);
}

```