#include <mbed.h>
#include "buzzer.h"

#include <threadLvgl.h>

#include "demos/lv_demos.h"
#include <cstdio>

#define I2C_ADDR 42 // 0x15 passage sur 8 bit
#define readDelay 5
#define measureDelay 1000

//I2C

uint8_t Tabwrite []={0x04,0x13,0x8B,0x00,0x01};

uint8_t Tabread [4];
//taille de l'écran 270 par 480

//Pinout Buzzer
PwmOut Buzzer(D3);


ThreadLvgl g_threadLvgl(30);

/*  ============
    Fonction I2C
    ============       
*/
I2C g_i2c(I2C_SDA, I2C_SCL);

uint16_t GetCO2PPM()
{
    //début transmission
    
    int r= g_i2c.write(I2C_ADDR, (char *)Tabwrite, sizeof(Tabwrite), false);

    printf("%d\n",r);
    ThisThread::sleep_for (10ms);

    g_i2c.read(I2C_ADDR, (char *)Tabread, sizeof(Tabread), false);

    printf("TABCO2:%02X %02X %02X %02X\n",(int)Tabread[0],(int)Tabread[1],(int)Tabread[2],(int)Tabread[3]);


    return (Tabread[2]<<8) | Tabread[3];
   // return ((Tabread[2] & 0x3F)<<8) | Tabread[3];

   
}
/*
Calcul du PPM= MSB*256 + LSB
04  02 MSB LSB
0 to 5000 ppm
400 PPM= valeur de base dehors
<400 PPM = Excellent
400<...<600 Bon
600<...<1200 Moyen
1200<...<2000 Mauvais
2000<...<5000 Critique

*/

/*  =================
    FIN Fonctionn I2C
    =================       
*/

int CalculPPM ( uint16_t * dest){

    uint8_t MSB = (*dest>>8)& 0xFF;
    uint8_t LSB = (*dest)& 0xFF;
    *dest = MSB *256 + LSB;
    printf("dest:%d\n",*dest);
    if (*dest<600){
        return 0;
    
    }
        if (*dest<1200){
        return 1;
    
    }
        if (*dest<2000){
        return 2;
    
    }
        if (*dest<5000){
        return 3;
    
    }
        return -1;

}



/*  =========
    Graphique
    =========       
*/
//static lv_obj_t * chart;
//static lv_chart_series_t * ser;
//static lv_chart_cursor_t * cursor;
//static lv_obj_t * label;
static lv_obj_t * labelText1;
static lv_obj_t * labelText3;
static lv_obj_t * labelText4;




/*
static void event_cb(lv_event_t * e)
{
    static int32_t last_id = -1;
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);

    if(code == LV_EVENT_VALUE_CHANGED) {
        last_id = lv_chart_get_pressed_point(obj);
        if(last_id != LV_CHART_POINT_NONE) {
            lv_chart_set_cursor_point(obj, cursor, NULL, last_id);
        }
    }
    else if(code == LV_EVENT_DRAW_PART_END) {
        lv_obj_draw_part_dsc_t * dsc = lv_event_get_draw_part_dsc(e);
        if(!lv_obj_draw_part_check_type(dsc, &lv_chart_class, LV_CHART_DRAW_PART_CURSOR)) return;
        if(dsc->p1 == NULL || dsc->p2 == NULL || dsc->p1->y != dsc->p2->y || last_id < 0) return;

        lv_coord_t * data_array = lv_chart_get_y_array(chart, ser);
        lv_coord_t v = data_array[last_id];
        char buf[16];
        lv_snprintf(buf, sizeof(buf), "%d", v);

        lv_point_t size;
        lv_txt_get_size(&size, buf, LV_FONT_DEFAULT, 0, 0, LV_COORD_MAX, LV_TEXT_FLAG_NONE);

        lv_area_t a;
        a.y2 = dsc->p1->y - 5;
        a.y1 = a.y2 - size.y - 10;
        a.x1 = dsc->p1->x + 10;
        a.x2 = a.x1 + size.x + 10;

        lv_draw_rect_dsc_t draw_rect_dsc;
        lv_draw_rect_dsc_init(&draw_rect_dsc);
        draw_rect_dsc.bg_color = lv_palette_main(LV_PALETTE_BLUE);
        draw_rect_dsc.radius = 3;

        lv_draw_rect(dsc->draw_ctx, &draw_rect_dsc, &a);

        lv_draw_label_dsc_t draw_label_dsc;
        lv_draw_label_dsc_init(&draw_label_dsc);
        draw_label_dsc.color = lv_color_white();
        a.x1 += 5;
        a.x2 -= 5;
        a.y1 += 5;
        a.y2 -= 5;
        lv_draw_label(dsc->draw_ctx, &draw_label_dsc, &a, buf, NULL);
    }
    
}*/
/*  =============
    Fin Graphique
    =============       
*/






/*  ========
    Bouton 1
    ========       
*/
static void btn1_event_cb(lv_event_t * e)
{



    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * btn = lv_event_get_target(e);
    if(code == LV_EVENT_CLICKED) {
        Buzzer.write(0.00f); //0.0 pour éteindre le buzzer   1.0 plus jamais

        /*Get the first child of the button which is the label and change its text*/
        lv_obj_t * button1 = lv_obj_get_child(btn, 0);
        lv_label_set_text(button1, "Arret du buzzer:");

/*  ==============
    Style bouton 1
    ==============      
*/
/*
        static lv_style_t scrStyle, debugStyle;
            lv_style_init(&scrStyle);
           
            lv_style_set_opa(&scrStyle, LV_OPA_100);

            lv_style_init(&debugStyle);
            lv_style_set_text_color(&debugStyle, lv_color_make(255, 0, 0));

            lv_obj_add_style(lv_scr_act(),&scrStyle, LV_STATE_DEFAULT);

            lv_obj_add_style(btn,&debugStyle, LV_STATE_DEFAULT);*/
/*  ==================
    FIN Style bouton 1
    ==================     
*/
    }
}
/*  ============
    Fin Bouton 1
    ============       
*/
/*  ========
    Bouton 2
    ========       
*/
static void btn2_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * btn2 = lv_event_get_target(e);
    if(code == LV_EVENT_CLICKED) {
        Buzzer.write(0.50f); //0.0 pour éteindre le buzzer   1.0 plus jamais

        /*Get the first child of the button which is the label and change its text*/
        lv_obj_t * button2 = lv_obj_get_child(btn2, 0);
        lv_label_set_text(button2, "Test du buzzer");

    }
}
/*  ============
    Fin Bouton 2
    ============       
*/

/*  ========
    Bouton 3
    ========       
*/
static void btn3_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * btn3 = lv_event_get_target(e);
    if(code == LV_EVENT_CLICKED) {

       //Afficher le Graphique avec l'appui sur le bouton
        // lv_obj_clear_flag(chart,LV_OBJ_FLAG_HIDDEN);
         //lv_obj_clear_flag(label,LV_OBJ_FLAG_HIDDEN);
         lv_obj_clear_flag(labelText4,LV_OBJ_FLAG_HIDDEN);
       
        lv_obj_add_flag(labelText3,LV_OBJ_FLAG_HIDDEN);

        /*Get the first child of the button which is the label and change its text*/
        lv_obj_t * button3 = lv_obj_get_child(btn3, 0);
        lv_label_set_text(button3, "Affichage des \n Seuils de PPM");

    }
}
/*  ============
    Fin Bouton 4
    ============       
*/

static void btn4_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * btn4 = lv_event_get_target(e);
    if(code == LV_EVENT_CLICKED) {

       //Affiche le profile avec l'appui sur le bouton
        
       //Cache le graphique
       // lv_obj_add_flag(chart,LV_OBJ_FLAG_HIDDEN);
       // lv_obj_add_flag(label,LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(labelText4,LV_OBJ_FLAG_HIDDEN);

        //affiche les label
        lv_obj_clear_flag(labelText3,LV_OBJ_FLAG_HIDDEN);
        
        

        uint16_t CO2PPM = GetCO2PPM();
            CalculPPM(&CO2PPM);

            char TABLEAU[60];
            snprintf(TABLEAU,sizeof(TABLEAU), "CO2 en temps reel: %d PPM",CO2PPM);
            lv_label_set_text(labelText3, TABLEAU);


            



        printf("%d\n",CO2PPM);
        //lv_obj_clear_flag(labelText2,LV_OBJ_FLAG_HIDDEN);
        /*Get the first child of the button which is the label and change its text*/
        lv_obj_t * button4 = lv_obj_get_child(btn4, 0);
        lv_label_set_text(button4, "Affichage du \n CO2");

    }
}
/*  ============
    Fin Bouton 4
    ============       
*/



int main() {

/*  ======
    BUZZER
    ======      
*/
Buzzer.period_us(50);
Buzzer.write(0.00f); //0.0 pour éteindre le buzzer   1.0 plus jamais

/*  ==========
    FIn BUZZER
    ==========      
*/

/*  ===============
    ===============    
    Début Interface
    ===============
    ===============        
*/
    g_threadLvgl.lock();

/*  ======
    TEXTE1
    ======      
*/
    labelText1 = lv_label_create(lv_scr_act());
    
    lv_label_set_text(labelText1, "Qualite de l'air");

     

/*  ==========
    FIN TEXTE1
    ==========    
*/

    

/*  ======
    TEXTE3
    ======      
*/


    labelText3 = lv_label_create(lv_scr_act());
    lv_label_set_text(labelText3, "");
    lv_obj_align(labelText3, LV_ALIGN_CENTER, -60,-60);
    //lv_obj_add_flag(labelText3,LV_OBJ_FLAG_HIDDEN);

/*  ==========
    FIN TEXTE3
    ==========    
*/
/*  ======
    TEXTE4
    ======      
*/


    labelText4 = lv_label_create(lv_scr_act());

    lv_label_set_text(labelText4, "Voici les seuils de PPM:\n\rPPM<400 c'est excellent\n\r400<PPM<600 c'est bon\n\r600<PPM<1200 c'est moyen\n\r1200<PPM<2000 c'est mauvais\n\r2000<PPM<5000 c'est critique");
    lv_obj_align(labelText4, LV_ALIGN_CENTER, -70,0);
    lv_obj_add_flag(labelText4,LV_OBJ_FLAG_HIDDEN);

/*  ==========
    FIN TEXTE4
    ==========    
*/


/*  =========
    Graphique
    =========       
*/
/*
    chart = lv_chart_create(lv_scr_act());
        lv_obj_set_size(chart, 260, 180); //Dimension du graphique     largeur,hauteur
        lv_obj_align(chart, LV_ALIGN_CENTER, -40,10);// Placement du graphique     Horizontal, Vertical
    
    //Pointe sur l'objet / quel axe / taille du + grand traits/ + petit traits / nbr de tick
        lv_chart_set_axis_tick(chart, LV_CHART_AXIS_PRIMARY_Y, 10, 5, 6, 5, true, 40);//
        lv_chart_set_axis_tick(chart, LV_CHART_AXIS_PRIMARY_X, 5, 5, 24, 1, true, 30);//

        lv_obj_add_event_cb(chart, event_cb, LV_EVENT_ALL, NULL);
        lv_obj_refresh_ext_draw_size(chart);

        cursor = lv_chart_add_cursor(chart, lv_palette_main(LV_PALETTE_BLUE), LV_DIR_LEFT | LV_DIR_BOTTOM);

        ser = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);
        uint32_t i;
        for(i = 0; i < 24; i++) {
            lv_chart_set_next_value(chart, ser, lv_rand(10, 90));
        }

        lv_chart_set_zoom_x(chart, 100);

        label = lv_label_create(lv_scr_act());
        lv_label_set_text(label, "Evolution du niveau de CO2");
        lv_obj_align_to(label, chart, LV_ALIGN_OUT_TOP_MID, 0, -5);

        */
/*  =============
    Fin Graphique
    =============       
*/
//Pour masquer le Graphique au début
  //  lv_obj_add_flag(chart,LV_OBJ_FLAG_HIDDEN);
  //  lv_obj_add_flag(label,LV_OBJ_FLAG_HIDDEN);




/*  ========
    Bouton 1
    ========       
*/
    
    lv_obj_t * btn = lv_btn_create(lv_scr_act());     /*Add a button the current screen*/
    lv_obj_set_pos(btn, 340, 13.75);                            /*Set its position*/
    lv_obj_set_size(btn, 130, 40);                          /*Set its size*/
    lv_obj_add_event_cb(btn, btn1_event_cb, LV_EVENT_ALL, NULL);           /*Assign a callback to the button*/

    lv_obj_t * button1 = lv_label_create(btn);          /*Add a label to the button*/
    lv_label_set_text(button1, "Arret du buzzer");                     /*Set the labels text*/
    lv_obj_center(button1);
/*  ============
    Fin Bouton 1
    ============       
*/
/*  ========
    Bouton 2
    ========       
*/
    
    lv_obj_t * btn2 = lv_btn_create(lv_scr_act());     /*Add a button the current screen*/
    lv_obj_set_pos(btn2, 340, 81.25);                            /*Set its position*/
    lv_obj_set_size(btn2, 130, 40);                          /*Set its size*/
    lv_obj_add_event_cb(btn2, btn2_event_cb, LV_EVENT_ALL, NULL);           /*Assign a callback to the button*/

    lv_obj_t * button2 = lv_label_create(btn2);          /*Add a label to the button*/
    lv_label_set_text(button2, "Test du buzzer");                     /*Set the labels text*/
    lv_obj_center(button2);
/*  ============
    Fin Bouton 2
    ============       
*/

/*  ========
    Bouton 3
    ========       
*/
    
    lv_obj_t * btn3 = lv_btn_create(lv_scr_act());     /*Add a button the current screen*/
    lv_obj_set_pos(btn3, 340, 148.75);                            /*Set its position*/
    lv_obj_set_size(btn3, 130, 40);                          /*Set its size*/
    lv_obj_add_event_cb(btn3, btn3_event_cb, LV_EVENT_ALL, NULL);           /*Assign a callback to the button*/

    lv_obj_t * button3 = lv_label_create(btn3);          /*Add a label to the button*/
    lv_label_set_text(button3, "Affichage des \n Seuils de PPM");                     /*Set the labels text*/
    lv_obj_center(button3);


/*  ============
    Fin Bouton 3
    ============       
*/

/*  ========
    Bouton 4
    ========       
*/
    
    lv_obj_t * btn4 = lv_btn_create(lv_scr_act());     /*Add a button the current screen*/
    lv_obj_set_pos(btn4, 340, 216.25);                            /*Set its position*/
    lv_obj_set_size(btn4, 130, 40);                          /*Set its size*/
    lv_obj_add_event_cb(btn4, btn4_event_cb, LV_EVENT_ALL, NULL);           /*Assign a callback to the button*/

    lv_obj_t * button4 = lv_label_create(btn4);          /*Add a label to the button*/
    lv_label_set_text(button4, "Affichage du \n CO2");                     /*Set the labels text*/
    lv_obj_center(button4);


/*  ============
    Fin Bouton 4
    ============       
*/


    g_threadLvgl.unlock();
/*  ============
    Fin Interfaceg_threadLvgl.unlock();
    ============        
*/


    while (1) {
    
        uint16_t CO2PPM = GetCO2PPM();
        int EtatCritique = CalculPPM(&CO2PPM);
        if(EtatCritique==0 || EtatCritique==1){
            Buzzer.write(0.00f);
        }
        if(EtatCritique==2){
           // printf("Etat Critique 2\n");
            Buzzer.write(0.50f);  
        }
        if(EtatCritique==3){
           // printf("Etat Critique 3\n");
            Buzzer.write(1.00f);  
        }


/*  ===============
    ===============    
    Début Interface
    ===============
    ===============        
*/
    g_threadLvgl.lock();

    g_threadLvgl.unlock();
/*  =============
    =============    
    Fin Interface
    =============
    =============
*/

        // put your main code here, to run repeatedly:
        ThisThread::sleep_for(10ms);
    }
}