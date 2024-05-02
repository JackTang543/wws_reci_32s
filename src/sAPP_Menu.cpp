#include "sAPP_Menu.h"
#include "sGraphic2D.h"



//todo 基于C++标准库的框架

MainMenuCfg_t menu_cfg = {0};

/**
 * @brief 显示主页
 */
static void ShowHomePage(){
    //显示背景图
    sG2D_DrawScreenByImg(sAPP_Font_Img_MainPage);

    //显示是否连接WiFi
    if(WiFi.isConnected()){
        sG2D_Draw8x8Icon(12,1,sAPP_Font_8x8Icon_Wifi);
    }else{
        sG2D_WriteString(12,1,"X");
    }

    //显示是否连接2.4GHz
    if(sAPP_2D4GHz_GetRSSI() == 1){
        sG2D_Draw8x8Icon(0,1,sAPP_Font_8x8Icon_2D4Full);
    }
    else if(sAPP_2D4GHz_GetRSSI() == 0){
        sG2D_Draw8x8Icon(0,1,sAPP_Font_8x8Icon_2D4Half);
    }
    else if(sAPP_2D4GHz_GetRSSI() == -1){
        
    }

    //显示时间
    char time_str[10] = {0};
    sAPP_Func_RTC_Get12HTime(time_str);
    sG2D_WriteString(40,1,time_str);

    //显示电量
    uint8_t bat_x = 101;
    static char bat_percent_str[10] = {0};
    uint8_t bat_percent = sAPP_ADC_GetBatPercent();
    if(bat_percent == 100){
        snprintf(bat_percent_str,sizeof(bat_percent_str),"100");
    }
    else if(bat_percent != 255){
        snprintf(bat_percent_str,sizeof(bat_percent_str),"%d%%",bat_percent);
    }
    if(bat_percent < 25){
        sG2D_Draw8x8Icon(bat_x + 0,1,sAPP_Font_8x8Icon_Bat0);
    }
    else if(bat_percent < 50){
        sG2D_Draw8x8Icon(bat_x + 0,1,sAPP_Font_8x8Icon_Bat1);
    }
    else if(bat_percent < 75){
        sG2D_Draw8x8Icon(bat_x + 0,1,sAPP_Font_8x8Icon_Bat2);
    }
    else if(bat_percent <= 100){
        sG2D_Draw8x8Icon(bat_x + 0,1,sAPP_Font_8x8Icon_Bat3);
    }
    sG2D_WriteString(bat_x + 9,1,bat_percent_str);

    //状态栏下划线
    sG2D_DrawHLine(0,128,10,1);
    
    //显示温度
    uint8_t temp_str_x = 22,temp_str_y = 18;
    sG2D_WriteString(temp_str_x +  0,temp_str_y +  0,"TEMP");
    static char temp_str[10] = {0};
    snprintf(temp_str,sizeof(temp_str),"%02.1f",g_data_packet_p1.aht10_temp);
    sG2D_WriteString(temp_str_x +  0,temp_str_y + 12,temp_str);
    sG2D_Draw8x8Icon(temp_str_x + 25,temp_str_y + 12,sAPP_Font_8x8Icon_degC);
    //画进度条线
    //把温度从-10到45的范围映射到0-30的范围
    uint8_t temp_bar_len = map(g_data_packet_p1.aht10_temp,-10,45,0,30);
    sG2D_DrawHLine(temp_str_x + 1,temp_str_x + 1 + temp_bar_len,temp_str_y + 9,1);

    //显示湿度
    uint8_t humi_str_x = 22,humi_str_y = 45;
    sG2D_WriteString(humi_str_x +  0,humi_str_y +  0,"HUMI");
    static char humi_str[10] = {0};
    snprintf(humi_str,sizeof(temp_str),"%02.1f",g_data_packet_p1.aht10_humi);
    sG2D_WriteString(humi_str_x +  0,humi_str_y + 12,humi_str);
    sG2D_WriteString(humi_str_x + 25,humi_str_y + 12,"%");
    //画进度条线
    //把湿度从10到100的范围映射到0-30的范围
    uint8_t humi_bar_len = map(g_data_packet_p1.aht10_humi,10,100,0,30);
    sG2D_DrawHLine(humi_str_x + 1,humi_str_x + 1 + humi_bar_len,humi_str_y + 9,1);

    //显示气压
    uint8_t pres_str_x = 88,pres_str_y = 18;
    sG2D_WriteString(pres_str_x + 0,pres_str_y + 0,"PRESS");
    static char pres_str[10] = {0};
    snprintf(pres_str,sizeof(temp_str),"%4.1f",g_data_packet_p1.bmp280_pres);
    sG2D_WriteString(pres_str_x + 0,pres_str_y + 12,pres_str);
    //sG2D_WriteString(pres_str_x + 25,pres_str_y + 12,"HPa");
    //画进度条线
    //把气压从980到1030的范围映射到0-30的范围
    uint8_t pres_bar_len = map(g_data_packet_p1.bmp280_pres,980,1030,0,30);
    sG2D_DrawHLine(pres_str_x + 1,pres_str_x + 1 + pres_bar_len,pres_str_y + 9,1);

    //显示亮度
    uint8_t light_str_x = 88,light_str_y = 45;
    sG2D_WriteString(light_str_x + 0,light_str_y + 0,"LIGHT");
    static char light_str[10] = {0};
    snprintf(light_str,sizeof(temp_str),"%4.1f",g_data_packet_p1.temt_mv);
    sG2D_WriteString(light_str_x + 0,light_str_y + 12,light_str);
    sG2D_WriteString(light_str_x + 25,light_str_y + 12,"%");
    //画进度条线
    //把光照从0到100的范围映射到0-30的范围
    uint8_t light_bar_len = map(g_data_packet_p1.temt_mv,0,100,0,30);
    sG2D_DrawHLine(light_str_x + 1,light_str_x + 1 + light_bar_len,light_str_y + 9,1);
}

//显示次要信息页面
static void ShowViceHomePage(){
    // sG2D_SetAllGRAM(0);
    // sG2D_DrawTriangle(1,2,7,5,1,8,1);
    // sG2D_WriteString(10,2,"VICE PAGE");
    // sG2D_RevRectArea(0,0,127,10);
}

//显示2.4Ghz信息页面
static void Show2D4InfoPage(){
    sG2D_DrawScreenByImg(sAPP_Font_Img_2D4GhzInfo);
}

//显示2.4Ghz设置页面
static void Show2D4SettingsPage(){
    sG2D_DrawScreenByImg(sAPP_Font_Img_2D4GhzSettings);
}

//显示WiFi信息页面
static void ShowWiFiInfoPage(){
    sG2D_DrawScreenByImg(sAPP_Font_Img_WifiInfo);
}

//显示WiFi设置页面
static void ShowWiFiSettingsPage(){
    sG2D_DrawScreenByImg(sAPP_Font_Img_WiFiSettings);
}

//显示设置页面
static void ShowSettingsPage(){
    sG2D_DrawScreenByImg(sAPP_Font_Img_Settings);
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
    {"2.4Ghz Info Page" ,exit_page_cb, exit_page_cb, TwoD4GHzInfoPage_Enter, NULL, NULL},
    {"2.4Ghz Settings"  ,exit_page_cb, exit_page_cb, Show2D4SettingsPage, NULL, NULL},
    {"WiFi Info Page"   ,exit_page_cb, exit_page_cb, WiFiInfoPage_Enter, NULL, NULL},
    {"WiFi Settings"    ,exit_page_cb, exit_page_cb, ShowWiFiInfoPage, NULL, NULL},
    {"Other Settings"   ,exit_page_cb, exit_page_cb, SettingsPage_Enter, NULL, NULL}
};

//选中Item发送消息给sGBD2
static void SelectedItem_SendMsg(){
    Serial.printf("SelectedItem_SendMsg\n");
    xSemaphoreGive(g_sapp_menu_item_selected_sem);
}

//取消选中Item发送消息给sGBD2
static void UnselectItem_SendMsg(){
    Serial.printf("UnselectItem_SendMsg\n");
    xSemaphoreGive(g_sapp_menu_item_unselect_sem);
}


//WiFi信息页的内容
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

//2.4GHz信息页的内容
MenuList_t TwoD4GHzInfoPageItem[] = {
    {"STATE:", NULL, NULL, NULL, NULL, NULL},
    {"RF CH:", NULL, NULL, NULL, NULL, NULL},
    {"RxPWR:", NULL, NULL, NULL, NULL, NULL},
    {"Rx P0:", NULL, NULL, NULL, NULL, NULL},
    {"Rx P1:", NULL, NULL, NULL, NULL, NULL},
    {"Rx P2:", NULL, NULL, NULL, NULL, NULL},
    {"Rx P3:", NULL, NULL, NULL, NULL, NULL},
    {"Rx P4:", NULL, NULL, NULL, NULL, NULL},
    {"Rx P5:", NULL, NULL, NULL, NULL, NULL},
    {"Tx P :", NULL, NULL, NULL, NULL, NULL},
};

//设置页的内容
MenuList_t SettingsPageItem[] = {
    {" > Screen Brightness", SelectedItem_SendMsg, UnselectItem_SendMsg, SettingsPage_Bright_Enter, NULL, NULL},
    {" > Screen Sleep", SelectedItem_SendMsg, UnselectItem_SendMsg, SettingsPage_Sleep_Enter, NULL, NULL},
    {" > Buzzer Volume", SelectedItem_SendMsg, UnselectItem_SendMsg, SettingsPage_Volume_Enter, NULL, NULL},
    {" > WS2812 Brightness", SelectedItem_SendMsg, UnselectItem_SendMsg, SettingsPage_WS2812_Enter, NULL, NULL},
    {" > WiFi Connect", SelectedItem_SendMsg, UnselectItem_SendMsg, SettingsPage_WiFi_Enter, NULL, NULL},
    {" > Sync NTP Time", SelectedItem_SendMsg, UnselectItem_SendMsg, SettingsPage_NTP_Enter, NULL, NULL},
};

MenuList_t SettingsPage_Bright_Item[] = {
    {"brightness adj:", NULL, NULL, NULL, NULL, NULL},
};

MenuList_t SettingsPage_Sleep_Item[] = {
    {"Screen Sleep:", NULL, NULL, NULL, NULL, NULL},
};

MenuList_t SettingsPage_Volume_Item[] = {
    {"Buzzer Volume:", NULL, NULL, NULL, NULL, NULL},
};

MenuList_t SettingsPage_WS2812_Item[] = {
    {"WS2812 Brightness:", NULL, NULL, NULL, NULL, NULL},
};

MenuList_t SettingsPage_WiFi_Item[] = {
    {"WiFi Connect:", NULL, NULL, NULL, NULL, NULL},
};

MenuList_t SettingsPage_NTP_Item[] = {
    {"Sync NTP Time:", NULL, NULL, NULL, NULL, NULL},
};





//进入WiFi信息页
void WiFiInfoPage_Enter(){
    //动态绑定菜单
    cotMenu_Bind(WiFiInfoPageItem,GET_MENU_NUM(WiFiInfoPageItem),ShowWiFiInfoMenu);
    Serial.printf("WiFiInfoPage_Enter\n");
}

void TwoD4GHzInfoPage_Enter(){
    cotMenu_Bind(TwoD4GHzInfoPageItem,GET_MENU_NUM(TwoD4GHzInfoPageItem),Show2D4GHzInfoMenu);
    Serial.printf("2D4GHzInfoPage_Enter\n");
}

void SettingsPage_Enter(){
    cotMenu_Bind(SettingsPageItem,GET_MENU_NUM(SettingsPageItem),ShowSettingsMenu);
    Serial.printf("SettingsPage_Enter\n");
}

void SettingsPage_Bright_Enter(){
    cotMenu_Bind(SettingsPage_Bright_Item,GET_MENU_NUM(SettingsPage_Bright_Item),ShowSettingsMenuBrightPage);
}

void SettingsPage_Sleep_Enter(){
    cotMenu_Bind(SettingsPage_Sleep_Item,GET_MENU_NUM(SettingsPage_Sleep_Item),ShowSettingsMenuSleepPage);
}

void SettingsPage_Volume_Enter(){
    cotMenu_Bind(SettingsPage_Volume_Item,GET_MENU_NUM(SettingsPage_Volume_Item),ShowSettingsMenuSleepPage);
}

void SettingsPage_WS2812_Enter(){
    cotMenu_Bind(SettingsPage_WS2812_Item,GET_MENU_NUM(SettingsPage_WS2812_Item),ShowSettingsMenuWS2812Page);
}

void SettingsPage_WiFi_Enter(){
    cotMenu_Bind(SettingsPage_WiFi_Item,GET_MENU_NUM(SettingsPage_WiFi_Item),ShowSettingsMenuWiFiPage);
}

void SettingsPage_NTP_Enter(){
    cotMenu_Bind(SettingsPage_NTP_Item,GET_MENU_NUM(SettingsPage_NTP_Item),ShowSettingsMenuSyncNTPPage);
}


//显示WiFi信息菜单
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
}

//显示2.4GHz信息菜单
void Show2D4GHzInfoMenu(MenuShow_t *ptS){
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
    sG2D_WriteString(15,3,"2.4GHz INFO");
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
    static char state[20] = "";
    ptS->pItemsExData[0] = (void*)state;
    
    //RF CH
    char rf_ch[20] = "";
    snprintf(rf_ch,20,"       %uMHz",g_si24r1_conf.rf_ch + 2400);
    ptS->pItemsExData[1] = (void*)rf_ch;
    //RxPWR
    char rf_pwr[20] = "";
    if(g_si24r1_conf.rf_pwr == RF_PWR_7DBM){
        snprintf(rf_pwr,20,"         +7dBm");
    }
    else if(g_si24r1_conf.rf_pwr == RF_PWR_4DBM){
        snprintf(rf_pwr,20,"         +4dBm");
    }
    else if(g_si24r1_conf.rf_pwr == RF_PWR_3DBM){
        snprintf(rf_pwr,20,"         +3dBm");
    }
    else if(g_si24r1_conf.rf_pwr == RF_PWR_1DBM){
        snprintf(rf_pwr,20,"         +1dBm");
    }
    else if(g_si24r1_conf.rf_pwr == RF_PWR_0DBM){
        snprintf(rf_pwr,20,"         +0dBm");
    }
    else if(g_si24r1_conf.rf_pwr == RF_PWR_N4DBM){
        snprintf(rf_pwr,20,"         -4dBm");
    }
    else if(g_si24r1_conf.rf_pwr == RF_PWR_N6DBM){
        snprintf(rf_pwr,20,"         -6dBm");
    }
    else if(g_si24r1_conf.rf_pwr == RF_PWR_N12DBM){
        snprintf(rf_pwr,20,"        -12dBm");
    }
    ptS->pItemsExData[2] = (void*)rf_pwr;
    //Rx P0
    char rx_p0[20] = "";
    snprintf(rx_p0,20,"  0x%0llX",g_si24r1_conf.addr.rx_addr_p0);
    ptS->pItemsExData[3] = (void*)rx_p0;
    //Rx P1
    char rx_p1[20] = "";
    snprintf(rx_p1,20,"  0x%0llX",g_si24r1_conf.addr.rx_addr_p1);
    ptS->pItemsExData[4] = (void*)rx_p1;
    //Rx P2
    char rx_p2[20] = "";
    snprintf(rx_p2,20,"          0x%0X",g_si24r1_conf.addr.rx_addr_p2);
    ptS->pItemsExData[5] = (void*)rx_p2;
    //Rx P3
    char rx_p3[20] = "";
    snprintf(rx_p3,20,"          0x%0X",g_si24r1_conf.addr.rx_addr_p3);
    ptS->pItemsExData[6] = (void*)rx_p3;
    //Rx P4
    char rx_p4[20] = "";
    snprintf(rx_p4,20,"          0x%0X",g_si24r1_conf.addr.rx_addr_p4);
    ptS->pItemsExData[7] = (void*)rx_p4;
    //Rx P5
    char rx_p5[20] = "";
    snprintf(rx_p5,20,"          0x%0X",g_si24r1_conf.addr.rx_addr_p5);
    ptS->pItemsExData[8] = (void*)rx_p5;
    //Tx P
    char tx_p[20] = "";
    snprintf(tx_p,20,"  0x%0llX",g_si24r1_conf.addr.tx_addr);
    ptS->pItemsExData[9] = (void*)tx_p;

    //绘制可滚动区
    for(menusize_t i = ptS->showBaseItem + 0; i < ptS->showBaseItem + maxItems && i < ptS->itemsNum; i++){
        //绘制行的标题
        sG2D_WriteString(0, y_offset + 1, ptS->pszItemsDesc[i]);
        sG2D_DrawHLine(0,128,y_offset + 10,1);
        //绘制行的数据
        if(ptS->pItemsExData[i] != NULL){
            sG2D_WriteString(40,y_offset + 1,(const char*)ptS->pItemsExData[i]);
        }

        //处理选中区域
        if(i == ptS->selectItem){
            sG2D_RevRectArea(0,y_offset - 1 ,128,y_offset + 9);
        }
        //下一行
        y_offset += line_height;
    }
}

//显示Settings菜单
void ShowSettingsMenu(MenuShow_t *ptS){
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
    sG2D_WriteString(15,3,"SETTINGS");
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
}



sAPP_Btns_Event_t btn_event;

static void DrawMidBar(uint8_t bar_x,uint8_t bar_y,uint8_t bar_width,float percent){
    uint8_t bar_height = 6;
    uint8_t bar_handle_width = 4,bar_handle_height = 4;
    //计算进度条位置
    uint8_t bar_pos = (uint8_t)((percent / 100.0f) * (float)bar_width);
    //进度条的条
    sG2D_DrawRectangle(bar_x,bar_y,bar_x + bar_width,bar_y + bar_height,0);
    //把从0到手柄的部分填满
    sG2D_DrawRectangle(bar_x,bar_y,bar_x + bar_pos,bar_y + bar_height,1);
    //画一个手柄
    sG2D_DrawRectangle(bar_x + bar_pos - bar_handle_width / 2,bar_y - bar_handle_height / 2 \
    ,bar_x + bar_pos + bar_handle_width / 2 ,bar_y + bar_height  + bar_handle_height / 2,0);
}


//显示设置->调整亮度页面
void ShowSettingsMenuBrightPage(MenuShow_t *ptS){
    //显示背景图
    sG2D_SetAllGRAM(0);
    sG2D_DrawScreenByImg(sAPP_Font_Img_ScrrenBrightness);

    //如果队列有数据(按键按下)注意这里是非阻塞读取
    if(xQueueReceive(g_sgbd2_ev_mq, &btn_event, 0) == pdTRUE){
        if(btn_event.btn_id == BTN_UP_ID){
            //限幅
            g_oled_bl_percent < 100? g_oled_bl_percent += 5 : g_oled_bl_percent = 100;
        }else if(btn_event.btn_id == BTN_DOWN_ID){
            g_oled_bl_percent > 0  ? g_oled_bl_percent -= 5 : g_oled_bl_percent = 0;
        }
        //设置亮度
        nvs.begin("wws_reci");
        nvs.putInt(SAPP_NVS_KEY_OLED_BL_PERCENT,g_oled_bl_percent);
        nvs.end();
        sDRV_GenOLED_SetBrightness(g_oled_bl_percent);
    }
    //画一个进度条
    uint8_t bar_x = 40; uint8_t bar_y = 42; uint8_t bar_width = 50;
    DrawMidBar(bar_x,bar_y,bar_width,g_oled_bl_percent);
    //在进度条右侧加一个数据显示
    sG2D_WriteNumber(bar_x + bar_width + 5,bar_y,g_oled_bl_percent);
    sG2D_WriteString(bar_x + bar_width + 25,bar_y,"%");
}

void ShowSettingsMenuSleepPage(MenuShow_t *ptS){
    //显示背景图
    sG2D_SetAllGRAM(0);
    sG2D_DrawScreenByImg(sAPP_Font_Img_ScrrenSleepTime);
}


void ShowSettingsMenuSleepTimePage(MenuShow_t *ptS){
    //显示背景图
    sG2D_SetAllGRAM(0);
    //sG2D_DrawScreenByImg(sAPP_Font_Img_ScrrenSleepTime);
}

void ShowSettingsMenuWS2812Page(MenuShow_t *ptS){
    sG2D_SetAllGRAM(0);
    sG2D_DrawScreenByImg(sAPP_Font_Img_LEDBrightnessAdj);

    //如果队列有数据(按键按下)注意这里是非阻塞读取
    if(xQueueReceive(g_sgbd2_ev_mq, &btn_event, 0) == pdTRUE){
        if(btn_event.btn_id == BTN_UP_ID){
            //限幅
            g_ws2812_bl_percent < 100? g_ws2812_bl_percent += 5 : g_ws2812_bl_percent = 100;
        }else if(btn_event.btn_id == BTN_DOWN_ID){
            g_ws2812_bl_percent > 0  ? g_ws2812_bl_percent -= 5 : g_ws2812_bl_percent = 0;
        }
        //首先把数据保存下来
        nvs.begin("wws_reci");
        nvs.putInt(SAPP_NVS_KEY_WS2812_BL_PERCENT,g_ws2812_bl_percent);
        nvs.end();
        //设置亮度
        sAPP_WS2812_SetBrightness(g_ws2812_bl_percent);
    }
    //画一个进度条
    uint8_t bar_x = 40; uint8_t bar_y = 42; uint8_t bar_width = 50;
    DrawMidBar(bar_x,bar_y,bar_width,g_ws2812_bl_percent);
    //在进度条右侧加一个数据显示
    sG2D_WriteNumber(bar_x + bar_width + 5,bar_y,g_ws2812_bl_percent);
    sG2D_WriteString(bar_x + bar_width + 25,bar_y,"%");
}

void ShowSettingsMenuWiFiPage(MenuShow_t *ptS){
    sG2D_SetAllGRAM(0);
    //首先检查WiFi是否连接以确定背景图
    if(WiFi.isConnected()){
        sG2D_DrawScreenByImg(sAPP_Font_Img_WiFiConnected);
    }else{
        sG2D_DrawScreenByImg(sAPP_Font_Img_WiFiDisconnect);
    }

    //画两个重叠矩形,让他看起来像个按键
    sG2D_DrawRectangle(52,37,122,65,1);
    sG2D_DrawRectangle(48,33,118,61,0);

    //画按键label
    sG2D_WriteString(50,37,WiFi_SSID);
    if(!WiFi.isConnected()){
        sG2D_WriteString(53,50,"  CONNECT");
        sG2D_RevRectArea(62,48,110,58);
    }else{
        sG2D_WriteString(53,50,"DISCONNECT");
        sG2D_RevRectArea(53,48,113,58);
    }
    
    //如果按键按下
    if(xQueueReceive(g_sgbd2_ev_mq, &btn_event, 0) == pdTRUE){
        if(btn_event.btn_id == BTN_ENTER_ID){
            //如果已经连接了,按下ENTER则断开
            if(WiFi.isConnected()){
                //断开WiFi,并关闭射频
                WiFi.disconnect(true);
            }else{
            //没有连接,按下ENTER连接,下面处理WiFi连接工作
                sG2D_DrawScreenByImg(sAPP_Font_Img_WiFiConnecting);
                sG2D_WriteString(53,50,"  Wait...");
                sG2D_RevRectArea(62,48,110,58);
                sG2D_UpdateScreen();
                WiFi.begin(WiFi_SSID, WiFi_PWD);
                //等待8s
                uint8_t i = 0;
                while (!WiFi.isConnected()) {
                    delay(1000); i++;    //等待10s
                    if(i > 10){break;}
                }
            }
        }
    }
}

void ShowSettingsMenuSyncNTPPage(MenuShow_t *ptS){
    sG2D_SetAllGRAM(0);
    sG2D_DrawScreenByImg(sAPP_Font_Img_SyncNTPTime);

    //画两个重叠矩形,让他看起来像个按键
    sG2D_DrawRectangle(52,50,122,65,1);
    sG2D_DrawRectangle(48,46,118,61,0);

    static struct tm timeinfo;
    getLocalTime(&timeinfo);
    static char time_str[20];
    sprintf(time_str,"%02d-%02d-%02d %02d:%02d:%02d",(timeinfo.tm_year + 1900) - 2000, \
    timeinfo.tm_mon + 1,timeinfo.tm_mday,timeinfo.tm_hour,timeinfo.tm_min,timeinfo.tm_sec);

    //画按键label
    sG2D_WriteString(43,33,time_str);
    if(WiFi.isConnected()){
        sG2D_WriteString(55,51,"Sync Time");
    }else{
        sG2D_WriteString(55,51," No WiFi!");
    }
    

    if(xQueueReceive(g_sgbd2_ev_mq, &btn_event, 0) == pdTRUE){
        if(btn_event.btn_id == BTN_ENTER_ID){
            if(WiFi.isConnected()){
                sAPP_Task_TIM_SyncTimeByNTP(NULL);
            }
        }
    }
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
