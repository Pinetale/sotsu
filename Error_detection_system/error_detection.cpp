//
//  erro_detection.c
//  Error_detection_system
//
//  Created by 松尾静一郎 on 2018/01/22.
//  Copyright © 2018年 松尾静一郎. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <iostream>

#define MAX_STR 20

// valid_distance = -1 : not consider distance between characters
unsigned int getNumRangeMatchChar( const std::string &s1, const std::string &s2, const int distance = -1 ){
    unsigned int l1 = s1.size();
    unsigned int l2 = s2.size();
    bool ignore_distance = false;
    
    if( distance < 0 ){ ignore_distance = true; }
    
    unsigned int counter = 0;
    for( unsigned int i = 0; i < l1; ++i ){
        
        // setting search range
        unsigned int from = i;
        unsigned int under = l2;
        if(! ignore_distance ){
            from = ( i < (unsigned int)distance ) ? 0 :  i - (unsigned int)distance;
            under = ( i +(unsigned int)distance >= l2 ) ? l2 : i + (unsigned int)distance;
        }
        for( unsigned int j = from; j < under; ++j ){
            if( s1[i] == s2[j] ){ ++counter; }
            // NOTICE : this code consider the case i == j ( both place and character match )
        }
    }
    return counter;
}

std::string getRangeMatchChar( const std::string &s1, const std::string &s2, const int distance = -1 ){
    unsigned int l1 = s1.size(), l2 = s2.size();
    bool ignore_distance = false;
    
    if( distance < 0 ){ ignore_distance = true; }
    
    std::string ret = "";
    
    for( unsigned int i = 0; i < l1; ++i ){
        // setting search range
        unsigned int from = i;
        unsigned int under = l2;
        if(! ignore_distance ){
            from = ( i < distance ) ? 0 :  i - distance;
            under = ( i + distance >= l2 ) ? l2 : i + distance;
        }
        for( unsigned int j = from; j < under; ++j ){
            if( s1[i] == s2[j] ){ ret += s1[i]; }
            // NOTICE : this code consider the case i == j ( both place and character match )
        }
    }
    return ret;
}

unsigned int getNumTransposition( const std::string &s1, const std::string &s2 ){
    unsigned int c = 0;
    for( unsigned int i = 0; i < s1.size() && i < s2.size(); ++i ){
        if( s1[i] != s2[i] ){ ++c; }
    }
    return c;
    
}

// Error the case  s1 & s2 are empty
double getJaroDistance( const std::string &s1, const std::string &s2 ){
    int distance = ( s1.size() > s2.size() ) ? s1.size() : s2.size();
    if( distance < 0 ){ return -1; }
    distance = distance / 2 - 1;
    if( distance < 0 ){ return -1; }
    
    int match = getNumRangeMatchChar( s1, s2, distance );
    int trans = getNumTransposition( getRangeMatchChar( s1, s2, distance ), getRangeMatchChar( s2, s1, distance ) );
    double m = (double)match;
    double t = ( (double)trans ) / 2.;
    
    return ( m / (double)s1.size() + m / (double)s2.size() + ( m - t ) / match ) / 3.;
}

unsigned int getLengthOfCommonPrefix( const std::string &s1, const std::string &s2 ){
    unsigned int c = 0;
    for( unsigned int i = 0; i < s1.size() && i < s2.size(); ++i ){
        if( s1[i] == s2[i] ){ ++c; } else { return c; }
    }
    return c;
}

double getJaroWinklerDistance( const std::string &s1, const std::string &s2, const double scaling = 0.1 ){
    if( scaling < 0 ){ return -1; }
    double j = getJaroDistance( s1, s2 );
    return j + getLengthOfCommonPrefix( s1, s2 ) * scaling * ( 1 - j );
}

/*
//string1とstring2を比較し、間違いの文字数を数える
int compare_strings(char *string1, char *string2){
    unsigned long len_string1 = strlen(string1);
    unsigned long len_string2 = strlen(string2);
    int min_len;
    int i = 0, unmatch_count = 0;
    
    unmatch_count = (len_string1 - len_string2);
    
    if (len_string1 < len_string2)
        min_len = len_string1;
    else
        min_len = len_string2;
    
    for(i=0; i<min_len; i++){
        if(string1[i] != string2[i])
            unmatch_count++;
    }
    return unmatch_count;
}
*/



int main(void) {
    FILE *fp;
    char chData;
    int row = 1; //行
    int col = 0; //列
    char **table[MAX_STR];
    int i,j,k,l,h;
    int t_row,t_col;
    char table_tmp[MAX_STR];
    char **learnMap[MAX_STR];
    
    fp = fopen("test-example.txt", "r");
    
    if(fp == NULL){
        printf("ファイルを読み込めませんでした。\n");
        return 0;
    }
    
    /* 1 文字ずつ読み込む  */
    while(fscanf(fp, "%c", &chData) != EOF){
        if('\n' == chData){
            row++;
        }
        if(',' == chData){
            col++;
        }
    }

        col = (col/row)+1;
    
        t_row = row; //rowをあとで使用するので
        t_col = col; //colも同様
    
    printf("読み込んだファイル\n");
    printf("行:%d,列:%d\n", t_row, t_col);
    
    fclose(fp);
    
    //table[i] = (char *)malloc(sizeof(char **) * MAX_STR);
    for (i = 0;i < row; i++) {
        table[i] = (char **)malloc(sizeof(char *) * col * MAX_STR);
    }
    
    
    for (i = 0; i < row; i++) {
        for (j = 0; j < col; j++) {
            table[i][j] = (char *)malloc(sizeof(char) * MAX_STR);
            //printf("%s\n", table[i][j]);
        }
    }
    
    
    for (i = 0;i < row; i++) {
        learnMap[i] = (char **)malloc(sizeof(char *) * col * MAX_STR);
    }
    

    for(i = 0;i < row; i++){
        for(j = 0; j < col; j++){
            learnMap[i][j] = (char *)malloc(sizeof(char) * MAX_STR);
        }
    }

    
    fp = fopen("test-example.txt", "r");
    
    memset(table_tmp,'\0',MAX_STR);
    
    //iの初期化
    i = 0;
    //rowとcolも上で数値が入れられている状態なので初期化
    row = 0;
    col = 0;
    
    while(fscanf(fp, "%c", &chData) != EOF){
        if(',' == chData){
            strcpy(table[row][col],table_tmp);
            col++;
            i = 0;
            memset(table_tmp,'\0',MAX_STR);
        }
        else if('\n' == chData){
            strcpy(table[row][col],table_tmp);
            row++;
            i = 0;
            col = 0;
            memset(table_tmp,'\0',MAX_STR);
        }
        else{
            table_tmp[i] = chData;
            i++;
            
        }
        
    }
    
   strcpy(table[row][col],table_tmp);
    
    
   // memset(learnMap, 'a', MAX_STR);
    
    //strcpy(learnMap[0], "aaa");
    
    for(i = 0; i < t_row; i++){
        for(j = 0;j < t_col; j++){
            memset(learnMap[i][j], '\0', MAX_STR);
            
        }
    }
    

    int flag = 0;
    h = 0;
    k = 0;
    
  //while(h < t_col){
while(h < t_col){
    for(i = 0; i < t_row; i++){
        for(j = i+1; j < t_row; j++){
            if(strcmp(table[i][h],table[j][h]) == 0){
                l=0;
                while(l < t_row){
                    if(strcmp(learnMap[l][h],table[i][h]) == 0){
                        flag =0;
                        break;
                    }
                        flag += abs(strcmp(learnMap[l][h],table[i][h]));
                        l++;
                    }
                    if(flag > 0){
                        strcpy(learnMap[k][h],table[i][h]);
                        k++;
                        flag = 0;
                        break;
                    }
                }
            }
        }
      h++;
    }

    
    
    for(i = 0; i < t_row; i++){
        for(j = 0; j < t_col; j++){
            printf("%s ", table[i][j]);
            
        }
        printf("\n");
    }
   /* strcpy(table[0][1], "Taro");
    printf("%s\n", table[0][1]); */
    
    printf("\n");
    printf("同一文字列\n");
    for(i = 0; i < k; i++){
        for(j = 0; j < h; j++){
            printf("%s ",learnMap[i][j]);
        }
       printf("\n");
    }
    
    printf("\n");
    //printf("%s\n", table[1][1]);
    
    double result;
    
    for(j = 0; j < k; j++){
        for(i = 1;i < t_col; i++){
            result = getJaroWinklerDistance(table[i][0], learnMap[j][0], 0.1);
            printf("\"%s\"|\"%s\" 類似度：%f\n",table[i][0],learnMap[j][0],result);
            
        }
        
    }
        
    
    
    
    
    
    //とりあえず長さをとってみた
    /*len = strlen(learnMap[0]);
    printf("長さ:%lu\n",len);
    
    printf("記憶文字列\"%s\"|比較文字列\"%s\"\n",learnMap[0],table[1][0]);
    printf("相違数:%d個\n" ,compare_strings(learnMap[0], table[1][0]));
    
    
    
    /*int flaggg = 0;
    for(i = 0;i < MAX_STR;i++){
        printf("%d|%d\n", learnMap[0][i], table[1][0][i]);
        if(learnMap[0][i] != table[1][0][i])
            flaggg = 1;
    }
        
    if(flaggg == 0)
        printf("同じ文字列です。\n");
    else
        printf("この文字列は同一ではありません。\n");*/
    
    
   /* for(i = 0;i < MAX_STR;i++){
        if(learnMap[0][i] == table[1][0][i]){
            printf("同じ文字列です。\n");
        }else{
            printf("この文字列は同一ではありません。\n");
        }
    } */
    
    
    
    for (i=0; i < row ; i++) {
        for(j = 0; j < col; j++){
           free(table[i][j]);
        }
    }

    for (i = 0;i < row; i++) {
        free(table[i]);
    }
    
    
    fclose(fp);
    return 0;
    

}

