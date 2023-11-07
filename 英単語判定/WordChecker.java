// このファイルはほぼ完成

import java.io.*;
import java.net.*;

public class WordChecker {
    static int DebugMode = 0; //if 1 then on. デバッグ情報をAdminのターミナルに出力します．
    String pastFst = " "; //デバッグモード時に使用
    int a; //デバッグモード時に使用
    String wordToCheck;
    static String[] words = new String[58109];
    int[] charSizeBox = { //アルファベットごとの配列の大きさ
        3479, 3210, 5493, 3776, 2588, 2557, 1836, 2026, 2637, 473, 354, 1837, 2944, 919, 1388, 4563, 290, 3634, 6670, 2881, 1921, 811, 1542, 14, 144, 86
    };
    int[] charIndex = { //アルファベットごとの配列の先頭index
        0, 3479, 6689, 12182, 15958, 18546, 21103, 22939, 24965, 27638, 28111, 28465, 30302, 33246, 34165, 35553, 40116, 40406, 44040, 50710, 53591, 55512, 56323, 57865, 57879, 58023
    };

    void WordCheckerInit(String theUrlString){
        int k=0;

        try {
            URL url = new URL(theUrlString);
            BufferedReader reader = new BufferedReader(new InputStreamReader(url.openStream()));
            while (true) {
                String Word = reader.readLine();
                if (k == words.length) {
                    if(DebugMode == 1) System.out.println("Word == NULL!");
                    reader.close();
                    break;
                }
                words[k] = Word;
                //if(DebugMode == 1) System.out.println(Word + "was inserted in " + k);
                if(DebugMode == 1) {
                    String fstLetter = Word.substring(0,1);
                    if(fstLetter.equals(pastFst)){
                        a++;
                    }else{
                        pastFst = fstLetter;
                        System.out.println("The word is " + Word + ", so FirstIndex should be: " + k);
                    }
                }
                k++;
            }
        } catch (MalformedURLException e) {
            System.out.println("URLの形式が誤っています：" + e);
        } catch (IOException e) {
            System.out.println("I/Oエラーです：" + e);
        }
    }

    // 入力した単語がWebサイト上に存在したらtrue, そうでなければfalseを返す関数．
    boolean wordCheck(String w, String first){
        wordToCheck = w.toUpperCase();
        byte[] firstNum = first.getBytes();
        int charSizeBoxIndex = firstNum[0] - 97; // A は　0　にマップ
        if(DebugMode == 1) System.out.println("charSizeBoxIndex: " + charSizeBoxIndex);
        int initIndex = 0;
        int charSize = charSizeBox[charSizeBoxIndex];

        while(true){
            //WordSearch.wordSearch(charIndex[charSizeBoxIndex], charSizeBox[charSizeBoxIndex] - 1, words, wordToCheck); //WordSearchでハッシュマップつかうならこう使う
            if(initIndex < charSize){
                if(wordToCheck.equals(words[charIndex[charSizeBoxIndex] + initIndex])){
                    if(DebugMode == 1) System.out.println("The compaired word in words[]: " + words[charIndex[charSizeBoxIndex] + initIndex]);
                    return true;
                }
                initIndex++;
            }else{
                break;
            }
        }

        return false;
    }
}