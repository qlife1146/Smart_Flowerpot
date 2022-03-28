#include <SoftwareSerial.h>
SoftwareSerial CC2541(12, 13);							// SoftwareSerial(TX, RX)
int led = 10;											// led 포트
//int maximum = 0;
int buttonState = 0;
int ledState = 0;

void setup() {
    Serial.begin(9600);
    CC2541.begin(9600);
    pinMode(9, OUTPUT);									// 릴레이 모듈 핀 번호
    pinMode(led, OUTPUT);								// led 세팅
    digitalWrite(9, HIGH);								// 수중 모터 디폴트 설정(OFF)
}

void loop() {
    // 변수
    int c = CC2541.read();								// 앱에서 받아온 값
    int a = analogRead(A2);								// 수위센서
    int b = analogRead(A1);								// 조도센서
    int d = analogRead(A0);								// 토양수분센서값
    Serial.print("수위: ");
    Serial.println(a);
    Serial.print("수분: ");
    Serial.println(d);

    /*  디버깅 파트
        Serial.print("moisture: ");
        Serial.println(d);
    */

    // 실시간 기능 파트
    // 토양수분 실시간 체크 후 펌프 작동(이상무)
    if (d <= 150) {										//토양수분이 150보다 낮을 때
        if (a <= 400) {									//물이 400보다 낮게 있으면
            digitalWrite(9, HIGH);						//펌프는 작동을 안 하고
			//아 물 없다고요ㅋㅋ라는 식의 알람 공간
														//알림을 내뱉는다

        } else {										//토양수분이 150보다 낮고, 물이 400보다 많으면
            digitalWrite(9, LOW);						//펌프가 켜진다
        }
    }

    else if (150 < d) {									//토양수분이 150보다 높으면
        digitalWrite(9, HIGH);							//펌프가 꺼진다
    }


    //조도 실시간 체크 후 LED 작동(이상무)
    if ( b >= 900) {									// 조도 센서 값이 900보다 작거나 같으면
        digitalWrite(led, HIGH);						// led 온(반대 개념. 밝을수록 수치가 낮아짐)
    }
    else {												// 조도 센서 값이 900보다 크면
        digitalWrite(led, LOW);							// led 오프
    }


    // 애플리케이션 기능 파트
    // 수위센서
    if ( c == '3') {									// 수위 센서 값
        if ( a < 400 ) {
            CC2541.print("부족");

        }
        if ( a > 400) {
            CC2541.print("충분");
        }

    }
    if ( c == '4') {									// 조도 센서 값
        ledState = digitalRead(led);

        if (ledState == LOW) {
            CC2541.print("LED OFF");
        }
        if (ledState == HIGH) {
            CC2541.print("LED ON");
        }
        //CC2541.print("조도 map : ");
        //CC2541.print(b_m);
        //Serial.print(b);
    }

    if ( c == '5') {									// 토양 수분 값
        if ( d < 300) {
            CC2541.print("건조함");
            digitalWrite(9, LOW);
            buttonState = digitalRead(9);
            if ( buttonState == LOW ) {
                CC2541.print("ON");
            }
        }
        if ( d >= 300) {								// 토양 수분 값이 200 넘으면
            digitalWrite(9, HIGH);
            CC2541.print("적당함");
            CC2541.print("OFF");
        }
    }
}
