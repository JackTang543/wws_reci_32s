#include "sAPP_Menu.h"
#include "sGraphic2D.h"



//todo 基于C++标准库的框架

MainMenuCfg_t menu_cfg = {0};

//显示主页
static void ShowHomePage(){
    sG2D_WriteString(0,0,"TRANS LINKED");
    sG2D_DrawHLine(0,127,8,1);
    static char buffer[32]; // 足够大的缓冲区来存储格式化的字符串

    // 显示温度
    snprintf(buffer, sizeof(buffer), "TEMP:  %.1f", data_packet_p1.aht10_temp);
    sG2D_WriteString(10, 15, buffer);
    sG2D_WriteString(100,15,"degC");
    // 显示湿度
    snprintf(buffer, sizeof(buffer), "HUMI:  %.1f", data_packet_p1.aht10_humi);
    sG2D_WriteString(10, 25, buffer);
    sG2D_WriteString(100,25,"%RH");
    // 显示气压（转换为百帕斯卡）
    snprintf(buffer, sizeof(buffer), "PRES:  %.2f", data_packet_p1.bmp280_pres);
    sG2D_WriteString(10, 35, buffer);
    sG2D_WriteString(100,35,"HPa");
    // 显示光照
    snprintf(buffer, sizeof(buffer), "LIGHT: %.2f", data_packet_p1.temt_mv);
    sG2D_WriteString(10, 45, buffer);
    sG2D_WriteString(100,45,"%");

    snprintf(buffer, sizeof(buffer), "%4.0fmV", bat_mv);
    sG2D_WriteString(10, 55, buffer);
    snprintf(buffer, sizeof(buffer), "%4.0fmA", bat_ma);
    sG2D_WriteString(50, 55, buffer);
}

//显示次要信息页面
static void ShowViceHomePage(){
    sG2D_SetAllGRAM(0);
    sG2D_DrawTriangle(1,2,7,5,1,8,1);
    sG2D_WriteString(10,2,"VICE PAGE");
    sG2D_RevRectArea(0,0,127,10);
}

//显示2.4Ghz信息页面
static void Show2D4InfoPage(){
    sG2D_DrawTriangle(1,2,7,5,1,8,1);
    sG2D_WriteString(10,2,"2.4GHz INFO");
    sG2D_RevRectArea(0,0,127,10);
}

//显示2.4Ghz设置页面
static void Show2D4SettingsPage(){
    sG2D_DrawTriangle(1,2,7,5,1,8,1);
    sG2D_WriteString(10,2,"2.4GHz SETTINGS");
    sG2D_RevRectArea(0,0,127,10);
}

//显示WiFi信息页面
static void ShowWiFiInfoPage(){
    sG2D_DrawTriangle(1,2,7,5,1,8,1);
    sG2D_WriteString(10,2,"WIFI INFO");
    //sG2D_RevRectArea(0,0,127,10);
}

//显示WiFi设置页面
static void ShowWiFiSettingsPage(){
    sG2D_DrawTriangle(1,2,7,5,1,8,1);
    sG2D_WriteString(10,2,"WIFI SETTINGS");
    sG2D_RevRectArea(0,0,127,10);
}

//显示设置页面
static void ShowSettingsPage(){
    sG2D_DrawTriangle(1,2,7,5,1,8,1);
    sG2D_WriteString(10,2,"SETTINGS");
    sG2D_RevRectArea(0,0,127,10);
}




void enter_cb(){
    Serial.println("enter menu\n");
}

void exit_page_cb(){
    sG2D_SetAllGRAM(0);
    Serial.println("exit menu\n");
}

void load_cb(){
    Serial.println("load menu\n");
}





MenuList_t tabMenuItems[] = {
    {"Home Page"        ,ShowViceHomePage, exit_page_cb, ShowHomePage, NULL, NULL},
    {"2.4Ghz Info Page" ,exit_page_cb, exit_page_cb, Show2D4InfoPage, NULL, NULL},
    {"2.4Ghz Settings"  ,exit_page_cb, exit_page_cb, Show2D4SettingsPage, NULL, NULL},
    {"WiFi Info Page"   ,exit_page_cb, exit_page_cb, WiFiInfoPage_Enter, NULL, NULL},
    {"WiFi Settings"    ,exit_page_cb, exit_page_cb, ShowWiFiInfoPage, NULL, NULL},
    {"Other Settings"   ,exit_page_cb, exit_page_cb, ShowSettingsPage, NULL, NULL}
};

typedef struct{
    char* data;
}ItemDescData_t;


MenuList_t WiFiInfoPageItem[] = {
    {"STAT:", NULL, NULL, NULL, NULL, NULL},
    {"RSSI:", NULL, NULL, NULL, NULL, NULL},
    {"IP  :", NULL, NULL, NULL, NULL, NULL},
    {"MAC :", NULL, NULL, NULL, NULL, NULL},
    {"SSID:", NULL, NULL, NULL, NULL, NULL},
    {"PWD :", NULL, NULL, NULL, NULL, NULL},
    {"CH  :", NULL, NULL, NULL, NULL, NULL},
    {"gwIP:", NULL, NULL, NULL, NULL, NULL}
};


void WiFiInfoPage_Enter(){
    cotMenu_Bind(WiFiInfoPageItem,GET_MENU_NUM(WiFiInfoPageItem),ShowWiFiInfoMenu);
    Serial.printf("WiFiInfoPage_Enter");
}

//显示WiFi信息菜单回调
void ShowWiFiInfoMenu(MenuShow_t *ptS){
    sG2D_SetAllGRAM(0);
    //最大显示数目4行+1行标题行
    menusize_t maxItems = 4;
    cotMenu_LimitShowListNum(ptS,&maxItems);
    //y轴偏移
    uint16_t y_offset = 0;
    //行高
    uint16_t line_height = 12;

    //画一个装饰三角形在标题栏左边
    sG2D_DrawTriangle(1,2,9,6,1,10,1);
    //标题字
    sG2D_WriteString(15,3,"WIFI INFO");
    //右侧装饰线1
    uint8_t line1_x = 100;
    sG2D_DrawLine(line1_x + 0,0,line1_x + 5 + 1,12,1);
    sG2D_DrawLine(line1_x + 1,0,line1_x + 5 + 2,12,1);
    sG2D_DrawLine(line1_x + 2,0,line1_x + 5 + 3,12,1);
    //右侧装饰线2
    uint8_t line2_x = 110;
    sG2D_DrawLine(line2_x + 0,0,line2_x + 5 + 1,12,1);
    sG2D_DrawLine(line2_x + 1,0,line2_x + 5 + 2,12,1);
    sG2D_DrawLine(line2_x + 2,0,line2_x + 5 + 3,12,1);

    //反转标题栏
    sG2D_RevRectArea(0,0,128,12);
    //让下面的操作不影响标题栏
    y_offset += line_height + 5;
    //第一个item的上边框线
    sG2D_DrawHLine(0,128,y_offset - 2,1);

    //获取数据
    //STATE
    if(WiFi.isConnected()){
        ptS->pItemsExData[0] = (void*)"       CONNECTED";
    }else{
        ptS->pItemsExData[0] = (void*)"    DISCONNECTED";
    }
    //RSSI
    char rssi[20] = "";
    if(WiFi.isConnected()){
        snprintf(rssi,20,"        %2ddBm",WiFi.RSSI());
        ptS->pItemsExData[1] = (void*)rssi;
    }else{
        ptS->pItemsExData[1] = (void*)"         NaN dBm";
    }
    //IP
    char ip[20] = "";
    if(WiFi.isConnected()){
        snprintf(ip,20,"%s",WiFi.localIP().toString().c_str());
        ptS->pItemsExData[2] = (void*)ip;
    }else{
        ptS->pItemsExData[2] = (void*)"             NaN";
    }
    //MAC
    char mac[20] = "";
    snprintf(mac,20,"%s",WiFi.macAddress().c_str());
    ptS->pItemsExData[3] = (void*)mac;
    //SSID
    ptS->pItemsExData[4] = (void*)WiFi_SSID;
    //PWD
    ptS->pItemsExData[5] = (void*)WiFi_PWD;
    //CHANNEL
    char ch[20] = "";
    if(WiFi.isConnected()){
        snprintf(ch,20,"            CH%02d",WiFi.channel());
        ptS->pItemsExData[6] = (void*)ch;
    }else{
        ptS->pItemsExData[6] = (void*)"             NaN";
    }
    //gatewayIP
    char gw_ip[20] = "";
    if(WiFi.isConnected()){
        snprintf(gw_ip,20,"%s",WiFi.gatewayIP().toString().c_str());
        ptS->pItemsExData[7] = (void*)gw_ip;
    }else{
        ptS->pItemsExData[7] = (void*)"             NaN";
    }
    
    //绘制可滚动区
    for(menusize_t i = ptS->showBaseItem + 0; i < ptS->showBaseItem + maxItems && i < ptS->itemsNum; i++){
        //绘制行的标题
        sG2D_WriteString(0, y_offset + 1, ptS->pszItemsDesc[i]);
        sG2D_DrawHLine(0,128,y_offset + 10,1);
        //绘制行的数据
        if(ptS->pItemsExData[i] != NULL){
            sG2D_WriteString(30,y_offset + 1,(const char*)ptS->pItemsExData[i]);
        }

        //处理选中区域
        if(i == ptS->selectItem){
            sG2D_RevRectArea(0,y_offset - 1 ,128,y_offset + 9);
        }
        //下一行
        y_offset += line_height;
    }

    //下面处理数据填充


    // //ShowWiFiInfoPage();
    // sG2D_DrawTriangle(1,2,7,5,1,8,1);
    // sG2D_WriteString(10,2,ptShowInfo->pszItemsDesc[0]);
    // //sG2D_RevRectArea(0,0,128,10);
    // //sG2D_DrawHLine(0,128,22,1);   
    
    // sG2D_WriteString(0,line_height * 1 + 1,ptShowInfo->pszItemsDesc[1]);
    // sG2D_WriteString(40,line_height * 1 + 1,"CONNECTED");

    // sG2D_DrawHLine(0,128,10 + line_height * 1,1);   

    // sG2D_WriteString(0,line_height * 2 + 1,ptShowInfo->pszItemsDesc[2]);
    // sG2D_WriteString(40,line_height * 2 + 1,"-76dBm");

    // sG2D_DrawHLine(0,128,10 + line_height * 2,1);  


    // sG2D_WriteString(0,line_height * 3 + 1,ptShowInfo->pszItemsDesc[3]);
    // sG2D_WriteString(40,line_height * 3 + 1,"192.168.1.109");
    
    // sG2D_DrawHLine(0,127,10 + line_height * 3,1);   

    // sG2D_WriteString(0,line_height * 4 + 1,ptShowInfo->pszItemsDesc[4]);
    // sG2D_WriteString(40,line_height * 4 + 1,"75:3E:9A:1C:08");
    
    // sG2D_DrawHLine(0,127,10 + line_height * 4,1);   


    // if(ptS->selectItem == 0){
    //     sG2D_RevRectArea(0,0,128,10);
    // }
    //sG2D_RevRectArea(0,(line_height * ptS->selectItem) - 1 ,128,(line_height * ptS->selectItem) + 9);
    

    // sG2D_WriteString(0,y_pos + 61,"ACCX :         1.753g");
    // sG2D_DrawHLine(0,127,y_pos + 70,1);   

    // sG2D_WriteString(0,y_pos + 73,"ACCY :         0.142g");
    // sG2D_DrawHLine(0,127,y_pos + 82,1);   

    // sG2D_WriteString(0,y_pos + 85,"ACCZ :         7.561g");
    // sG2D_DrawHLine(0,127,y_pos + 94,1);   
}


// 显示菜单信息的回调
void ShowMainMenu(MenuShow_t *ptShowInfo){
    sG2D_SetAllGRAM(0);

    //Item0:主页
    if(ptShowInfo->selectItem == 0){
        ShowHomePage();
    }
    //2.4GHz信息
    else if(ptShowInfo->selectItem == 1){
        Show2D4InfoPage();
    }
    //2.4GHz设置
    else if(ptShowInfo->selectItem == 2){
        Show2D4SettingsPage();
    }
    //WiFi信息
    else if(ptShowInfo->selectItem == 3){
        ShowWiFiInfoPage();
    }
    //WiFi设置
    else if(ptShowInfo->selectItem == 4){
        ShowWiFiSettingsPage();
    }
    //其他设置
    else if(ptShowInfo->selectItem == 5){
        ShowSettingsPage();
    }
    // Serial.println(ptShowInfo->pszDesc);
    // for (int i = 0; i < ptShowInfo->itemsNum; i++) {
    //     Serial.print(i == ptShowInfo->selectItem ? "> " : "  ");
    //     Serial.println(ptShowInfo->pszItemsDesc[i]);
    // }
}



void sAPP_Menu_Init(){
    
    menu_cfg.pszDesc[0] = "Main Menu";
    menu_cfg.pfnEnterCallFun = enter_cb;
    menu_cfg.pfnExitCallFun = exit_page_cb;
    menu_cfg.pfnLoadCallFun = ShowHomePage;
    menu_cfg.pfnRunCallFun = NULL;    
    
    cotMenu_Init(&menu_cfg);
    cotMenu_SelectLanguage(0);

    cotMenu_Bind(tabMenuItems,GET_MENU_NUM(tabMenuItems),ShowMainMenu);

    

    //cotMenu_Bind(subMenuItems, GET_MENU_NUM(subMenuItems), showMenu);

    cotMenu_MainEnter();

    Serial.println("cot menu init done\n");

    

    

}







void sAPP_Menu_Handler(){
    cotMenu_Task();
}


// void sAPP_Menu_ShowHome(){
//     sG2D_WriteString(0,0,"TRANS LINKED");
//     sG2D_DrawHLine(0,127,8,1);
//     static char buffer[32]; // 足够大的缓冲区来存储格式化的字符串

//     // 显示温度
//     snprintf(buffer, sizeof(buffer), "TEMP:  %.1f", data_packet_p1.aht10_temp);
//     sG2D_WriteString(10, 15, buffer);
//     sG2D_WriteString(100,15,"degC");

//     // 显示湿度
//     snprintf(buffer, sizeof(buffer), "HUMI:  %.1f", data_packet_p1.aht10_humi);
//     sG2D_WriteString(10, 25, buffer);
//     sG2D_WriteString(100,25,"%RH");

//     // 显示气压（转换为百帕斯卡）
//     snprintf(buffer, sizeof(buffer), "PRES:  %.2f", data_packet_p1.bmp280_pres);
//     sG2D_WriteString(10, 35, buffer);
//     sG2D_WriteString(100,35,"HPa");

//     // 显示光照
//     snprintf(buffer, sizeof(buffer), "LIGHT: %.2f", data_packet_p1.temt_mv); // 假设光照单位为Lux，且整数足够
//     sG2D_WriteString(10, 45, buffer);
//     sG2D_WriteString(100,45,"%");

//     snprintf(buffer, sizeof(buffer), "%4.0fmV", bat_mv);
//     sG2D_WriteString(10, 55, buffer);

//     snprintf(buffer, sizeof(buffer), "%4.0fmA", bat_ma);
//     sG2D_WriteString(50, 55, buffer);

// }





// void sAPP_Menu_ShowInfo(){

//     sG2D_DrawTriangle(1,y_pos + 2,7,y_pos + 5,1,y_pos + 8,1);
//     sG2D_WriteString(10,y_pos + 2,"WIFI INFO");
//     // sG2D_DrawHLine(0,127,9,1);
//     // sG2D_DrawHLine(0,127,10,1);

//     sG2D_RevRectArea(0,y_pos + 0,127,y_pos + 10);

//     sG2D_WriteString(0,y_pos + 13,"STATE:      CONNECTED");
//     sG2D_DrawHLine(0,127,y_pos + 22,1);   

//     sG2D_WriteString(0,y_pos + 25,"IP   :  192.168.1.104");
//     sG2D_DrawHLine(0,127,y_pos + 34,1);   

//     sG2D_WriteString(0,y_pos + 37,"MAC  : 74:4F:98:4A:8B");
//     sG2D_DrawHLine(0,127,y_pos + 46,1);   

//     sG2D_WriteString(0,y_pos + 49,"RSSI :         -76dBm");
//     sG2D_DrawHLine(0,127,y_pos + 58,1);   

//     sG2D_WriteString(0,y_pos + 61,"ACCX :         1.753g");
//     sG2D_DrawHLine(0,127,y_pos + 70,1);   

//     sG2D_WriteString(0,y_pos + 73,"ACCY :         0.142g");
//     sG2D_DrawHLine(0,127,y_pos + 82,1);   

//     sG2D_WriteString(0,y_pos + 85,"ACCZ :         7.561g");
//     sG2D_DrawHLine(0,127,y_pos + 94,1);   

//     //sG2D_DrawHLine(0,127,y_pos + 63,1);   
// }


// void sAPP_Menu_Handler(){
//     if(page == 1){
//         sAPP_Menu_ShowHome();
//     }
//     else if(page == 2){
//         sAPP_Menu_ShowInfo();
//         //sG2D_WriteString(10,30,"THIS IS PAGE 2");
//     }
//     else if(page == 3){
//         sG2D_WriteString(10,10,"THIS IS PAGE 3");
//     }
//     else if(page == 4){
//         sG2D_WriteString(10,10,"THIS IS PAGE 4");
//     }
//     else if(page == 5){
//         sG2D_WriteString(10,10,"THIS IS PAGE 5");
//     }
    
// }

