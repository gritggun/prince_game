## 3D 다이아몬드 게임

제작년도 : 2006  
제작버전 : Visual Studio 6  
프로젝트 : 3D 다이아몬드  
팀　　명 : Carpe Diem  
팀　　원 : 김정수, 권기학, 김찬현, 안재훈

![prince_game](https://user-images.githubusercontent.com/112194029/189132153-c06ffee6-7287-41f3-803d-26e8a828edc8.jpg)


2006년도 숭실대학교 미디어학과 고급프로그래밍 수업 프로젝트 과제로 제출한 작품입니다. 

MFC를 한창 배울때이고 게임을 만들어본 경험이 거의 없던지라 팀원들과 고생고생 하면서 만든 기억이 납니다. 올린 버전은 풀 버전은 아니며, 풀버전은 게임 인트로, 네트워크 설정, 대화방, 멀티플레이 게임등 다양한 기능을 제공하는데 멀티플레이 게임 부분을 그때당시 시간이 부족하여 미완성으로 남기게 되었습니다.

단순히, 현재 버전에서는 개인 플레이만 가능합니다.  


## 게임 특징
### 3D 공간(?) 을 활용한 원근감 표현
![prince_game_01](https://user-images.githubusercontent.com/112194029/189133100-dfa7991f-fdb8-462e-b069-bd12db14fc7a.jpg)

### 구슬 애니메이션 
![prince_game_02](https://user-images.githubusercontent.com/112194029/189133105-11cf2377-68bf-49d1-a796-798ad6e1f428.jpg)

### 2~6인 플레이
![prince_game_03](https://user-images.githubusercontent.com/112194029/189133107-afee57e6-b399-479c-9370-f316454b168a.jpg)

### 아이템전
![prince_game_04](https://user-images.githubusercontent.com/112194029/189133108-382b6cd9-2e38-4c3f-a81d-b6686abcfa75.jpg)

### 타임오버
![prince_game_05](https://user-images.githubusercontent.com/112194029/189133110-e9130e47-7e5c-4692-9599-f5c9ef962142.jpg)

### 게임오버
![prince_game_06](https://user-images.githubusercontent.com/112194029/189133112-5ff71447-7c8e-4ab8-82fe-fb53c1295914.jpg)



## 게임설정 정보(생성자)
```c++
m_nWaitCount = 0;          // 대기 시간 카운트 
m_nPlayer = 6;             // 플레이어 수
m_nStats = 0;              // 클릭 상태
m_nWait = 10;              // 대기 타임
m_bCheckMove = FALSE;      // ┏━━━━━━━━━━┓
m_bCheckType = FALSE;      // ┃                 
m_bCheckFire = FALSE;      // ┃                 
m_bCheckWaitAni = FALSE;   // ┃ 이 변수들은 특정  
m_bCheckGameOver = FALSE;  // ┃ 이벤트시 발생하는 
m_bCheckItemCount = TRUE;  // ┃ 애니메이션을      
m_bCheckWaitCount = TRUE;  // ┃ 컨트롤 하는 변수  
m_bCheckStartCount = TRUE; // ┃ 들이다.          
m_bCheckOverCount = TRUE;  // ┃                 
m_bCheckFireCount = TRUE;  // ┃                 
m_bCheckStart = TRUE;      // ┗━━━━━━━━━━┛ 
m_bCheckItem = TRUE;       // 아이템전 체크
m_bCheckWait = TRUE;       // 대기시간 체크
m_nCurType = HUMAN_RED;    // 처음 시작할 구슬
```