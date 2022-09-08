// PrinceGameDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PrinceGame.h"
#include "PrinceGameDlg.h"
#include "Math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

// ���� ���¸� ��Ÿ��
#define HUMAN_RED		3
#define HUMAN_YELLOW	4
#define HUMAN_GREEN		5
#define HUMAN_CYAN		6
#define HUMAN_BLUE		7
#define HUMAN_PINK		8

#define EMPTY		10		// �ƹ��͵� ���� ���
#define OFFBOARD	11		// ���� ��輱
#define ACTIVATE1	12		// 1ĭ �̵��ؾ� �ϴ� ���
#define ACTIVATE2	13		// 2ĭ �̵��ؾ� �ϴ� ���

// Timer ID
#define REALDRAW	0		// ȭ���� �׷��� (�ִϸ��̼� ����)
#define WAITTIME	1		// ���ð� Ÿ�̸�

static int nWaitCount;		// ��� �ð� ī��Ʈ 

// ���� �Լ�
static void PlaySound(LPCTSTR lpszSound)
{
      HRSRC hRes; // resource handle to wave file
      HGLOBAL hData;
      BOOL bOk = FALSE;
      if ((hRes = ::FindResource(AfxGetResourceHandle(), lpszSound, _T("WAVE"))) 
            !=NULL && (hData = ::LoadResource(AfxGetResourceHandle(), hRes)) != NULL)
      {
            bOk=sndPlaySound((LPCTSTR)::LockResource(hData),SND_MEMORY|SND_ASYNC|SND_NODEFAULT);
            FreeResource(hData);
      }
}

inline static void PlaySound(UINT nIDS)
{ 
      PlaySound(MAKEINTRESOURCE(nIDS)); 
}

/////////////////////////////////////////////////////////////////////////////

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrinceGameDlg dialog

CPrinceGameDlg::CPrinceGameDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPrinceGameDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPrinceGameDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	// Todo (���� �ʱ�ȭ)
	nWaitCount = 0;				// ��� �ð� ī��Ʈ 
	m_nPlayer = 6;				// �÷��̾� ��
	m_nStats = 0;				// Ŭ�� ����
	m_nWait = 10;				// ��� Ÿ��
	m_bCheckMove = FALSE;		// ������������������������
	m_bCheckType = FALSE;		// ��                    ��
	m_bCheckFire = FALSE;		// ��                    ��
	m_bCheckWaitAni = FALSE;	// �� �� �������� Ư��   ��
	m_bCheckGameOver = FALSE;	// �� �̺�Ʈ�� �߻��ϴ�  ��
	m_bCheckItemCount = TRUE;	// �� �ִϸ��̼���       ��
	m_bCheckWaitCount = TRUE;	// �� ��Ʈ�� �ϴ� ����   ��
	m_bCheckStartCount = TRUE;	// �� ���̴�.            ��
	m_bCheckOverCount = TRUE;	// ��                    ��
	m_bCheckFireCount = TRUE;	// ��                    ��
	m_bCheckStart = TRUE;		// ������������������������	
	m_bCheckItem = TRUE;		// �������� üũ
	m_bCheckWait = TRUE;		// ���ð� üũ
	m_nCurType = HUMAN_RED;		// ó�� ������ ����

	// ������ ���� �ʱ�ȭ
	m_bCurItem[0] = TRUE;	m_bCurItem[1] = TRUE;
	m_bCurItem[2] = TRUE;	m_bCurItem[3] = TRUE;
	m_bCurItem[4] = TRUE;

	// ������ �߻� �ʱ�ȭ
	m_bEventItem[0] = FALSE;	m_bEventItem[1] = FALSE;
	m_bEventItem[2] = FALSE;	m_bEventItem[3] = FALSE;
	m_bEventItem[4] = FALSE;

	// ���� ����
	srand(time(NULL));			// �̰� ������ �������� �׻� ����
	m_nRand = rand()%3 + 1;		// ������ ���� 1, 2 ,3�� ���� ���ڴ�

	// ���� �ȼ� ��ǥ �ʱ�ȭ 
	// (�� �κ��� ������ Release ��� ����� �̻����� �߻�)
	int i, j;
	for(i=0; i<29; i++) {
		for(j=0; j<21; j++) {
			m_rectArray[i][j].left=0;	
			m_rectArray[i][j].top=0;	
			m_rectArray[i][j].right=0;	
			m_rectArray[i][j].bottom=0;
		}
	}

	// ���� �ȼ� ��ǥ (���� ������ �簢�� ũ�⿡�� �� +2px ��)
	m_rectArray[14][2].left=540;	m_rectArray[14][2].top=49;		m_rectArray[14][2].right=559;	m_rectArray[14][2].bottom=64;
	m_rectArray[13][3].left=526;	m_rectArray[13][3].top=68;		m_rectArray[13][3].right=546;	m_rectArray[13][3].bottom=85;
	m_rectArray[15][3].left=556;	m_rectArray[15][3].top=68;		m_rectArray[15][3].right=574;	m_rectArray[15][3].bottom=85;
	m_rectArray[12][4].left=512;	m_rectArray[12][4].top=89;		m_rectArray[12][4].right=530;	m_rectArray[12][4].bottom=104;
	m_rectArray[14][4].left=541;	m_rectArray[14][4].top=89;		m_rectArray[14][4].right=561;	m_rectArray[14][4].bottom=104;
	m_rectArray[16][4].left=571;	m_rectArray[16][4].top=89;		m_rectArray[16][4].right=590;	m_rectArray[16][4].bottom=104;
	m_rectArray[11][5].left=495;	m_rectArray[11][5].top=108;		m_rectArray[11][5].right=515;	m_rectArray[11][5].bottom=125;
	m_rectArray[13][5].left=526;	m_rectArray[13][5].top=108;		m_rectArray[13][5].right=546;	m_rectArray[13][5].bottom=125;
	m_rectArray[15][5].left=555;	m_rectArray[15][5].top=108;		m_rectArray[15][5].right=576;	m_rectArray[15][5].bottom=125;
	m_rectArray[17][5].left=586;	m_rectArray[17][5].top=108;		m_rectArray[17][5].right=605;	m_rectArray[17][5].bottom=125;

	m_rectArray[2][6].left=358;		m_rectArray[2][6].top=130;		m_rectArray[2][6].right=376;	m_rectArray[2][6].bottom=146;
	m_rectArray[4][6].left=388;		m_rectArray[4][6].top=130;		m_rectArray[4][6].right=407;	m_rectArray[4][6].bottom=146;
	m_rectArray[6][6].left=419;		m_rectArray[6][6].top=130;		m_rectArray[6][6].right=437;    m_rectArray[6][6].bottom=146;
	m_rectArray[8][6].left=449;		m_rectArray[8][6].top=130;		m_rectArray[8][6].right=468;	m_rectArray[8][6].bottom=146;
	m_rectArray[10][6].left=480;	m_rectArray[10][6].top=130;		m_rectArray[10][6].right=499;	m_rectArray[10][6].bottom=146;
	m_rectArray[12][6].left=511;	m_rectArray[12][6].top=130;		m_rectArray[12][6].right=529;	m_rectArray[12][6].bottom=146;
	m_rectArray[14][6].left=541;	m_rectArray[14][6].top=130;		m_rectArray[14][6].right=560;	m_rectArray[14][6].bottom=146;
	m_rectArray[16][6].left=572;	m_rectArray[16][6].top=130;		m_rectArray[16][6].right=591;	m_rectArray[16][6].bottom=146;
	m_rectArray[18][6].left=602;	m_rectArray[18][6].top=130;		m_rectArray[18][6].right=621;	m_rectArray[18][6].bottom=146;
	m_rectArray[20][6].left=633;	m_rectArray[20][6].top=130;		m_rectArray[20][6].right=652;	m_rectArray[20][6].bottom=146;

	m_rectArray[22][6].left=663;	m_rectArray[22][6].top=130;		m_rectArray[22][6].right=682;	m_rectArray[22][6].bottom=146;
	m_rectArray[24][6].left=694;	m_rectArray[24][6].top=130;		m_rectArray[24][6].right=713;	m_rectArray[24][6].bottom=146;
	m_rectArray[26][6].left=725;	m_rectArray[26][6].top=130;		m_rectArray[26][6].right=744;	m_rectArray[26][6].bottom=146;
	m_rectArray[3][7].left=370;		m_rectArray[3][7].top=152;		m_rectArray[3][7].right=389;	m_rectArray[3][7].bottom=169;
	m_rectArray[5][7].left=401;		m_rectArray[5][7].top=152;	    m_rectArray[5][7].right=420;	m_rectArray[5][7].bottom=169;
	m_rectArray[7][7].left=432;		m_rectArray[7][7].top=152;	    m_rectArray[7][7].right=451;	m_rectArray[7][7].bottom=169;
	m_rectArray[9][7].left=463;		m_rectArray[9][7].top=152;	    m_rectArray[9][7].right=482;	m_rectArray[9][7].bottom=169;
	m_rectArray[11][7].left=494;	m_rectArray[11][7].top=152;		m_rectArray[11][7].right=513;	m_rectArray[11][7].bottom=169;
	m_rectArray[13][7].left=525;	m_rectArray[13][7].top=152;		m_rectArray[13][7].right=544;	m_rectArray[13][7].bottom=169;
	m_rectArray[15][7].left=556;	m_rectArray[15][7].top=152;		m_rectArray[15][7].right=575;	m_rectArray[15][7].bottom=169;

	m_rectArray[17][7].left=587;	m_rectArray[17][7].top=152;		m_rectArray[17][7].right=606;	m_rectArray[17][7].bottom=169;
	m_rectArray[19][7].left=618;	m_rectArray[19][7].top=152;		m_rectArray[19][7].right=637;	m_rectArray[19][7].bottom=169;
	m_rectArray[21][7].left=649;	m_rectArray[21][7].top=152;		m_rectArray[21][7].right=668;	m_rectArray[21][7].bottom=169;
	m_rectArray[23][7].left=680;	m_rectArray[23][7].top=152;		m_rectArray[23][7].right=699;	m_rectArray[23][7].bottom=169;
	m_rectArray[25][7].left=711;	m_rectArray[25][7].top=152;		m_rectArray[25][7].right=730;	m_rectArray[25][7].bottom=169;
	m_rectArray[4][8].left=383;		m_rectArray[4][8].top=174;		m_rectArray[4][8].right=402;	m_rectArray[4][8].bottom=192;
	m_rectArray[6][8].left=415;		m_rectArray[6][8].top=174;		m_rectArray[6][8].right=434;	m_rectArray[6][8].bottom=192;
	m_rectArray[8][8].left=446;		m_rectArray[8][8].top=174;		m_rectArray[8][8].right=466;	m_rectArray[8][8].bottom=192;
	m_rectArray[10][8].left=478;	m_rectArray[10][8].top=174;		m_rectArray[10][8].right=497;	m_rectArray[10][8].bottom=192;
	m_rectArray[12][8].left=509;	m_rectArray[12][8].top=174;		m_rectArray[12][8].right=529;	m_rectArray[12][8].bottom=192;

	m_rectArray[14][8].left=541;	m_rectArray[14][8].top=174;		m_rectArray[14][8].right=560;	m_rectArray[14][8].bottom=192;
	m_rectArray[16][8].left=572;	m_rectArray[16][8].top=174;		m_rectArray[16][8].right=592;	m_rectArray[16][8].bottom=192;
	m_rectArray[18][8].left=604;	m_rectArray[18][8].top=174;		m_rectArray[18][8].right=623;	m_rectArray[18][8].bottom=192;
	m_rectArray[20][8].left=636;	m_rectArray[20][8].top=174;		m_rectArray[20][8].right=655;	m_rectArray[20][8].bottom=192;
	m_rectArray[22][8].left=667;	m_rectArray[22][8].top=174;		m_rectArray[22][8].right=687;	m_rectArray[22][8].bottom=192;
	m_rectArray[24][8].left=699;	m_rectArray[24][8].top=174;		m_rectArray[24][8].right=718;	m_rectArray[24][8].bottom=192;
	m_rectArray[5][9].left=396;		m_rectArray[5][9].top=198;	    m_rectArray[5][9].right=416;	m_rectArray[5][9].bottom=215;
	m_rectArray[7][9].left=428;		m_rectArray[7][9].top=198;	    m_rectArray[7][9].right=448;	m_rectArray[7][9].bottom=215;
	m_rectArray[9][9].left=460;		m_rectArray[9][9].top=198;	    m_rectArray[9][9].right=480;	m_rectArray[9][9].bottom=215;
	m_rectArray[11][9].left=492;	m_rectArray[11][9].top=198;		m_rectArray[11][9].right=512;	m_rectArray[11][9].bottom=215;

	m_rectArray[13][9].left=524;	m_rectArray[13][9].top=198;		m_rectArray[13][9].right=544;	m_rectArray[13][9].bottom=215;
	m_rectArray[15][9].left=556;	m_rectArray[15][9].top=198;		m_rectArray[15][9].right=576;	m_rectArray[15][9].bottom=215;
	m_rectArray[17][9].left=588;	m_rectArray[17][9].top=198;		m_rectArray[17][9].right=608;	m_rectArray[17][9].bottom=215;
	m_rectArray[19][9].left=620;	m_rectArray[19][9].top=198;		m_rectArray[19][9].right=640;	m_rectArray[19][9].bottom=215;
	m_rectArray[21][9].left=652;	m_rectArray[21][9].top=198;		m_rectArray[21][9].right=672;	m_rectArray[21][9].bottom=215;
	m_rectArray[23][9].left=684;	m_rectArray[23][9].top=198;		m_rectArray[23][9].right=704;	m_rectArray[23][9].bottom=215;
	m_rectArray[6][10].left=410;	m_rectArray[6][10].top=221;		m_rectArray[6][10].right=430;	m_rectArray[6][10].bottom=240;
	m_rectArray[8][10].left=443;	m_rectArray[8][10].top=221;		m_rectArray[8][10].right=463;	m_rectArray[8][10].bottom=240;
	m_rectArray[10][10].left=475;	m_rectArray[10][10].top=221;	m_rectArray[10][10].right=495;	m_rectArray[10][10].bottom=240;
	m_rectArray[12][10].left=508;	m_rectArray[12][10].top=221;	m_rectArray[12][10].right=528;	m_rectArray[12][10].bottom=240;

	m_rectArray[14][10].left=541;	m_rectArray[14][10].top=221;	m_rectArray[14][10].right=561;	m_rectArray[14][10].bottom=240;
	m_rectArray[16][10].left=573;	m_rectArray[16][10].top=221;	m_rectArray[16][10].right=593;	m_rectArray[16][10].bottom=240;
	m_rectArray[18][10].left=606;	m_rectArray[18][10].top=221;	m_rectArray[18][10].right=626;	m_rectArray[18][10].bottom=240;
	m_rectArray[20][10].left=638;	m_rectArray[20][10].top=221;	m_rectArray[20][10].right=658;	m_rectArray[20][10].bottom=240;
	m_rectArray[22][10].left=671;	m_rectArray[22][10].top=221;	m_rectArray[22][10].right=691;	m_rectArray[22][10].bottom=240;
	m_rectArray[5][11].left=391;	m_rectArray[5][11].top=246;		m_rectArray[5][11].right=412;	m_rectArray[5][11].bottom=265;
	m_rectArray[7][11].left=424;	m_rectArray[7][11].top=246;		m_rectArray[7][11].right=445;	m_rectArray[7][11].bottom=265;
	m_rectArray[9][11].left=456;	m_rectArray[9][11].top=246;		m_rectArray[9][11].right=477;	m_rectArray[9][11].bottom=265;
	m_rectArray[11][11].left=488;	m_rectArray[11][11].top=246;	m_rectArray[11][11].right=509;	m_rectArray[11][11].bottom=265;
	m_rectArray[13][11].left=520;	m_rectArray[13][11].top=246;	m_rectArray[13][11].right=541;	m_rectArray[13][11].bottom=265;

	m_rectArray[15][11].left=556;	m_rectArray[15][11].top=245;	m_rectArray[15][11].right=579;	m_rectArray[15][11].bottom=266;
	m_rectArray[17][11].left=589;	m_rectArray[17][11].top=245;	m_rectArray[17][11].right=612;	m_rectArray[17][11].bottom=266;
	m_rectArray[19][11].left=623;	m_rectArray[19][11].top=245;	m_rectArray[19][11].right=644;	m_rectArray[19][11].bottom=266;
	m_rectArray[21][11].left=655;	m_rectArray[21][11].top=245;	m_rectArray[21][11].right=678;	m_rectArray[21][11].bottom=266;
	m_rectArray[23][11].left=689;	m_rectArray[23][11].top=246;	m_rectArray[23][11].right=711;	m_rectArray[23][11].bottom=266;
	m_rectArray[4][12].left=370;	m_rectArray[4][12].top=271;	    m_rectArray[4][12].right=393;	m_rectArray[4][12].bottom=292;
	m_rectArray[6][12].left=405;	m_rectArray[6][12].top=271;		m_rectArray[6][12].right=427;	m_rectArray[6][12].bottom=292;
	m_rectArray[8][12].left=438;	m_rectArray[8][12].top=271;		m_rectArray[8][12].right=460;	m_rectArray[8][12].bottom=292;
	m_rectArray[10][12].left=472;	m_rectArray[10][12].top=271;	m_rectArray[10][12].right=495;	m_rectArray[10][12].bottom=292;
	m_rectArray[12][12].left=505;	m_rectArray[12][12].top=271;	m_rectArray[12][12].right=528;	m_rectArray[12][12].bottom=292;

	m_rectArray[14][12].left=539;	m_rectArray[14][12].top=271;	m_rectArray[14][12].right=562;	m_rectArray[14][12].bottom=292;
	m_rectArray[16][12].left=574;	m_rectArray[16][12].top=271;	m_rectArray[16][12].right=596;	m_rectArray[16][12].bottom=292;
	m_rectArray[18][12].left=607;	m_rectArray[18][12].top=271;	m_rectArray[18][12].right=630;	m_rectArray[18][12].bottom=292;
	m_rectArray[20][12].left=641;	m_rectArray[20][12].top=271;	m_rectArray[20][12].right=664;	m_rectArray[20][12].bottom=292;
	m_rectArray[22][12].left=674;	m_rectArray[22][12].top=271;	m_rectArray[22][12].right=697;	m_rectArray[22][12].bottom=292;
	m_rectArray[24][12].left=708;	m_rectArray[24][12].top=271;	m_rectArray[24][12].right=731;	m_rectArray[24][12].bottom=292;
	m_rectArray[3][13].left=349;	m_rectArray[3][13].top=298;		m_rectArray[3][13].right=374;	m_rectArray[3][13].bottom=319;
	m_rectArray[5][13].left=384;	m_rectArray[5][13].top=298;		m_rectArray[5][13].right=408;	m_rectArray[5][13].bottom=319;
	m_rectArray[7][13].left=418;	m_rectArray[7][13].top=298;		m_rectArray[7][13].right=442;	m_rectArray[7][13].bottom=319;
	m_rectArray[9][13].left=453;	m_rectArray[9][13].top=298;		m_rectArray[9][13].right=477;	m_rectArray[9][13].bottom=319;

	m_rectArray[11][13].left=487;	m_rectArray[11][13].top=298;	m_rectArray[11][13].right=511;	m_rectArray[11][13].bottom=319;
	m_rectArray[13][13].left=522;	m_rectArray[13][13].top=298;	m_rectArray[13][13].right=546;	m_rectArray[13][13].bottom=319;
	m_rectArray[15][13].left=556;	m_rectArray[15][13].top=298;	m_rectArray[15][13].right=580;	m_rectArray[15][13].bottom=319;
	m_rectArray[17][13].left=591;	m_rectArray[17][13].top=298;	m_rectArray[17][13].right=615;	m_rectArray[17][13].bottom=319;
	m_rectArray[19][13].left=624;	m_rectArray[19][13].top=298;	m_rectArray[19][13].right=649;	m_rectArray[19][13].bottom=319;
	m_rectArray[21][13].left=659;	m_rectArray[21][13].top=298;	m_rectArray[21][13].right=683;	m_rectArray[21][13].bottom=319;
	m_rectArray[23][13].left=693;	m_rectArray[23][13].top=298;	m_rectArray[23][13].right=717;	m_rectArray[23][13].bottom=319;
	m_rectArray[25][13].left=728;	m_rectArray[25][13].top=298;	m_rectArray[25][13].right=752;	m_rectArray[25][13].bottom=319;
	m_rectArray[2][14].left=329;	m_rectArray[2][14].top=325;		m_rectArray[2][14].right=354;	m_rectArray[2][14].bottom=347;
	m_rectArray[4][14].left=364;	m_rectArray[4][14].top=325;		m_rectArray[4][14].right=388;	m_rectArray[4][14].bottom=347;

	m_rectArray[6][14].left=399;	m_rectArray[6][14].top=325;		m_rectArray[6][14].right=423;	m_rectArray[6][14].bottom=347;
	m_rectArray[8][14].left=434;	m_rectArray[8][14].top=325;		m_rectArray[8][14].right=458;	m_rectArray[8][14].bottom=347;
	m_rectArray[10][14].left=469;	m_rectArray[10][14].top=325;	m_rectArray[10][14].right=493;	m_rectArray[10][14].bottom=347;
	m_rectArray[12][14].left=504;	m_rectArray[12][14].top=325;	m_rectArray[12][14].right=528;	m_rectArray[12][14].bottom=347;
	m_rectArray[14][14].left=539;	m_rectArray[14][14].top=325;	m_rectArray[14][14].right=563;	m_rectArray[14][14].bottom=347;
	m_rectArray[16][14].left=574;	m_rectArray[16][14].top=325;	m_rectArray[16][14].right=598;	m_rectArray[16][14].bottom=347;
	m_rectArray[18][14].left=608;	m_rectArray[18][14].top=325;	m_rectArray[18][14].right=633;	m_rectArray[18][14].bottom=347;
	m_rectArray[20][14].left=643;	m_rectArray[20][14].top=325;	m_rectArray[20][14].right=668;	m_rectArray[20][14].bottom=347;
	m_rectArray[22][14].left=679;	m_rectArray[22][14].top=325;	m_rectArray[22][14].right=703;	m_rectArray[22][14].bottom=347;
	m_rectArray[24][14].left=714;	m_rectArray[24][14].top=325;	m_rectArray[24][14].right=738;	m_rectArray[24][14].bottom=347;

	m_rectArray[26][14].left=748;	m_rectArray[26][14].top=325;	m_rectArray[26][14].right=772;	m_rectArray[26][14].bottom=347;
	m_rectArray[11][15].left=486;	m_rectArray[11][15].top=354;	m_rectArray[11][15].right=510;	m_rectArray[11][15].bottom=376;
	m_rectArray[13][15].left=521;	m_rectArray[13][15].top=354;	m_rectArray[13][15].right=545;	m_rectArray[13][15].bottom=376;
	m_rectArray[15][15].left=556;	m_rectArray[15][15].top=354;	m_rectArray[15][15].right=580;	m_rectArray[15][15].bottom=376;
	m_rectArray[17][15].left=592;	m_rectArray[17][15].top=354;	m_rectArray[17][15].right=617;	m_rectArray[17][15].bottom=376;
	m_rectArray[12][16].left=502;	m_rectArray[12][16].top=383;	m_rectArray[12][16].right=526;	m_rectArray[12][16].bottom=407;
	m_rectArray[14][16].left=539;	m_rectArray[14][16].top=383;	m_rectArray[14][16].right=563;	m_rectArray[14][16].bottom=407;
	m_rectArray[16][16].left=574;	m_rectArray[16][16].top=383;	m_rectArray[16][16].right=598;	m_rectArray[16][16].bottom=407;
	m_rectArray[13][17].left=520;	m_rectArray[13][17].top=414;	m_rectArray[13][17].right=544;	m_rectArray[13][17].bottom=437;
	m_rectArray[15][17].left=557;	m_rectArray[15][17].top=414;	m_rectArray[15][17].right=581;	m_rectArray[15][17].bottom=437;

	m_rectArray[14][18].left=539;	m_rectArray[14][18].top=445;	m_rectArray[14][18].right=563;	m_rectArray[14][18].bottom=469;
}

void CPrinceGameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPrinceGameDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPrinceGameDlg, CDialog)
	//{{AFX_MSG_MAP(CPrinceGameDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	ON_WM_NCHITTEST()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrinceGameDlg message handlers

BOOL CPrinceGameDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	SetWindowText("3D ���̾Ƹ��");

	LoadSkin();		// ��Ų �ε�
	InitGame();		// ���� �ʱ�ȭ

	// Sound Start
	PlaySound(IDR_START);

	// ���� �̹��� �ε�
	m_bmpRed[0].LoadBitmap(IDB_RLEVEL1);	m_bmpRed[1].LoadBitmap(IDB_RLEVEL2);	m_bmpRed[2].LoadBitmap(IDB_RLEVEL3);	m_bmpRed[3].LoadBitmap(IDB_RLEVEL4);
	m_bmpYellow[0].LoadBitmap(IDB_YLEVEL1);	m_bmpYellow[1].LoadBitmap(IDB_YLEVEL2);	m_bmpYellow[2].LoadBitmap(IDB_YLEVEL3);	m_bmpYellow[3].LoadBitmap(IDB_YLEVEL4);
	m_bmpGreen[0].LoadBitmap(IDB_GLEVEL1);	m_bmpGreen[1].LoadBitmap(IDB_GLEVEL2);	m_bmpGreen[2].LoadBitmap(IDB_GLEVEL3);	m_bmpGreen[3].LoadBitmap(IDB_GLEVEL4);
	m_bmpCyan[0].LoadBitmap(IDB_CLEVEL1);	m_bmpCyan[1].LoadBitmap(IDB_CLEVEL2);	m_bmpCyan[2].LoadBitmap(IDB_CLEVEL3);	m_bmpCyan[3].LoadBitmap(IDB_CLEVEL4);
	m_bmpBlue[0].LoadBitmap(IDB_BLEVEL1);	m_bmpBlue[1].LoadBitmap(IDB_BLEVEL2);	m_bmpBlue[2].LoadBitmap(IDB_BLEVEL3);	m_bmpBlue[3].LoadBitmap(IDB_BLEVEL4);
	m_bmpPink[0].LoadBitmap(IDB_PLEVEL1);	m_bmpPink[1].LoadBitmap(IDB_PLEVEL2);	m_bmpPink[2].LoadBitmap(IDB_PLEVEL3);	m_bmpPink[3].LoadBitmap(IDB_PLEVEL4);
	m_bmpMask[0].LoadBitmap(IDB_LEVELM1);	m_bmpMask[1].LoadBitmap(IDB_LEVELM2);	m_bmpMask[2].LoadBitmap(IDB_LEVELM3);	m_bmpMask[3].LoadBitmap(IDB_LEVELM4);
	
	// ������ �̹��� �ε�
	m_bmpItem[0].LoadBitmap(IDB_ITEM01);	m_bmpItem[1].LoadBitmap(IDB_ITEM02);
	m_bmpItem[2].LoadBitmap(IDB_ITEM03);	m_bmpItem[3].LoadBitmap(IDB_ITEM04);
	m_bmpItemMask[0].LoadBitmap(IDB_ITEM01M);	m_bmpItemMask[1].LoadBitmap(IDB_ITEM02M);
	m_bmpItemMask[2].LoadBitmap(IDB_ITEM03M);	m_bmpItemMask[3].LoadBitmap(IDB_ITEM04M);

	// Ÿ�ӿ��� �̹��� �ε�
	m_bmpWait.LoadBitmap(IDB_TIMEOVER);
	m_bmpWaitMask.LoadBitmap(IDB_TIMEOVERMASK);

	// ��ŸƮ �̹��� �ε�
	m_bmpStart.LoadBitmap(IDB_START);
	m_bmpStartMask.LoadBitmap(IDB_STARTMASK);

	// ���ӿ��� �̹��� �ε�
	m_bmpOver.LoadBitmap(IDB_OVER);
	m_bmpOverMask.LoadBitmap(IDB_OVERMASK);
	m_bmpFire.LoadBitmap(IDB_FIRE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPrinceGameDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPrinceGameDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		// RealDrawObject();

		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPrinceGameDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CPrinceGameDlg::BitmapRgn(UINT resource, COLORREF TansColor)
{
	HBITMAP			m_hBack;
	HINSTANCE hInstance = AfxGetInstanceHandle();
	m_hBack = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(resource));
	::SetWindowRgn( m_hWnd, BitmapToRegion( m_hBack, TansColor, TansColor ),TRUE);

	return TRUE;
}

HRGN CPrinceGameDlg::BitmapToRegion(HBITMAP hBmp, COLORREF cTransparentColor, COLORREF cTolerance)
{
	HRGN hRgn = NULL;

	if (hBmp)
	{
		// Create a memory DC inside which we will scan the bitmap content
		HDC hMemDC = CreateCompatibleDC(NULL);
		if (hMemDC)
		{
			// Get bitmap size
			BITMAP bm;
			GetObject(hBmp, sizeof(bm), &bm);

			// Create a 32 bits depth bitmap and select it into the memory DC 
			BITMAPINFOHEADER RGB32BITSBITMAPINFO = {	
					sizeof(BITMAPINFOHEADER),	// biSize 
					bm.bmWidth,					// biWidth; 
					bm.bmHeight,				// biHeight; 
					1,							// biPlanes; 
					32,							// biBitCount 
					BI_RGB,						// biCompression; 
					0,							// biSizeImage; 
					0,							// biXPelsPerMeter; 
					0,							// biYPelsPerMeter; 
					0,							// biClrUsed; 
					0							// biClrImportant; 
			};
			VOID * pbits32; 
			HBITMAP hbm32 = CreateDIBSection(hMemDC, (BITMAPINFO *)&RGB32BITSBITMAPINFO, DIB_RGB_COLORS, &pbits32, NULL, 0);
			if (hbm32)
			{
				HBITMAP holdBmp = (HBITMAP)SelectObject(hMemDC, hbm32);

				// Create a DC just to copy the bitmap into the memory DC
				HDC hDC = CreateCompatibleDC(hMemDC);
				if (hDC)
				{
					// Get how many bytes per row we have for the bitmap bits (rounded up to 32 bits)
					BITMAP bm32;
					GetObject(hbm32, sizeof(bm32), &bm32);
					while (bm32.bmWidthBytes % 4)
						bm32.bmWidthBytes++;

					// Copy the bitmap into the memory DC
					HBITMAP holdBmp = (HBITMAP)SelectObject(hDC, hBmp);
					BitBlt(hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, hDC, 0, 0, SRCCOPY);

					// For better performances, we will use the ExtCreateRegion() function to create the
					// region. This function take a RGNDATA structure on entry. We will add rectangles by
					// amount of ALLOC_UNIT number in this structure.
					#define ALLOC_UNIT	100
					DWORD maxRects = ALLOC_UNIT;
					HANDLE hData = GlobalAlloc(GMEM_MOVEABLE, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects));
					RGNDATA *pData = (RGNDATA *)GlobalLock(hData);
					pData->rdh.dwSize = sizeof(RGNDATAHEADER);
					pData->rdh.iType = RDH_RECTANGLES;
					pData->rdh.nCount = pData->rdh.nRgnSize = 0;
					SetRect(&pData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);

					// Keep on hand highest and lowest values for the "transparent" pixels
					BYTE lr = GetRValue(cTransparentColor);
					BYTE lg = GetGValue(cTransparentColor);
					BYTE lb = GetBValue(cTransparentColor);
					BYTE hr = min(0xff, lr + GetRValue(cTolerance));
					BYTE hg = min(0xff, lg + GetGValue(cTolerance));
					BYTE hb = min(0xff, lb + GetBValue(cTolerance));

					// Scan each bitmap row from bottom to top (the bitmap is inverted vertically)
					BYTE *p32 = (BYTE *)bm32.bmBits + (bm32.bmHeight - 1) * bm32.bmWidthBytes;
					for (int y = 0; y < bm.bmHeight; y++)
					{
						// Scan each bitmap pixel from left to right
						for (int x = 0; x < bm.bmWidth; x++)
						{
							// Search for a continuous range of "non transparent pixels"
							int x0 = x;
							LONG *p = (LONG *)p32 + x;
							while (x < bm.bmWidth)
							{
								BYTE b = GetRValue(*p);
								if (b >= lr && b <= hr)
								{
									b = GetGValue(*p);
									if (b >= lg && b <= hg)
									{
										b = GetBValue(*p);
										if (b >= lb && b <= hb)
											// This pixel is "transparent"
											break;
									}
								}
								p++;
								x++;
							}

							if (x > x0)
							{
								// Add the pixels (x0, y) to (x, y+1) as a new rectangle in the region
								if (pData->rdh.nCount >= maxRects)
								{
									GlobalUnlock(hData);
									maxRects += ALLOC_UNIT;
									hData = GlobalReAlloc(hData, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), GMEM_MOVEABLE);
									pData = (RGNDATA *)GlobalLock(hData);
								}
								RECT *pr = (RECT *)&pData->Buffer;
								SetRect(&pr[pData->rdh.nCount], x0, y, x, y+1);
								if (x0 < pData->rdh.rcBound.left)
									pData->rdh.rcBound.left = x0;
								if (y < pData->rdh.rcBound.top)
									pData->rdh.rcBound.top = y;
								if (x > pData->rdh.rcBound.right)
									pData->rdh.rcBound.right = x;
								if (y+1 > pData->rdh.rcBound.bottom)
									pData->rdh.rcBound.bottom = y+1;
								pData->rdh.nCount++;

								// On Windows98, ExtCreateRegion() may fail if the number of rectangles is too
								// large (ie: > 4000). Therefore, we have to create the region by multiple steps.
								if (pData->rdh.nCount == 2000)
								{
									HRGN h = ExtCreateRegion(NULL, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), pData);
									if (hRgn)
									{
										CombineRgn(hRgn, hRgn, h, RGN_OR);
										DeleteObject(h);
									}
									else
										hRgn = h;
									pData->rdh.nCount = 0;
									SetRect(&pData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);
								}
							}
						}

						// Go to next row (remember, the bitmap is inverted vertically)
						p32 -= bm32.bmWidthBytes;
					}

					// Create or extend the region with the remaining rectangles
					HRGN h = ExtCreateRegion(NULL, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), pData);
					if (hRgn)
					{
						CombineRgn(hRgn, hRgn, h, RGN_OR);
						DeleteObject(h);
					}
					else
						hRgn = h;

					// Clean up
					GlobalFree(hData);
					SelectObject(hDC, holdBmp);
					DeleteDC(hDC);
				}

				DeleteObject(SelectObject(hMemDC, holdBmp));
			}

			DeleteDC(hMemDC);
		}	
	}

	return hRgn;
}

void CPrinceGameDlg::LoadSkin()
{
	m_bmpDlg.LoadBitmap( IDB_BGSKIN01 );

	BITMAP bmp;
	m_bmpDlg.GetBitmap( &bmp );

	SetWindowPos( NULL, 0, 0, bmp.bmWidth, bmp.bmHeight, SWP_NOZORDER );
	// ��Ʈ�� ����ũ �����..
	BitmapRgn( IDB_BGSKINMASK, RGB( 255,255,255 ) );

	CenterWindow();
}

BOOL CPrinceGameDlg::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rc;
	GetClientRect(&rc);

	CDC MemDC;	
	MemDC.CreateCompatibleDC(pDC);

	CBitmap* pbmpOld = MemDC.SelectObject( &m_bmpDlg );		
	pDC->BitBlt(0, 0, rc.right, rc.bottom, &MemDC, 0, 0, SRCCOPY);

	MemDC.SelectObject( pbmpOld );
	ReleaseDC(pDC);
	
	return CDialog::OnEraseBkgnd(pDC);
}

void CPrinceGameDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	// ��Ʈ�� �ε��Ų�� �޸� ����
	DeleteObject(m_bmpDlg); 
	for(int i=0; i<4; i++) {
		DeleteObject(m_bmpRed[i]);
		DeleteObject(m_bmpYellow[i]);
		DeleteObject(m_bmpGreen[i]);
		DeleteObject(m_bmpCyan[i]);
		DeleteObject(m_bmpBlue[i]);
		DeleteObject(m_bmpPink[i]);
		DeleteObject(m_bmpMask[i]);
		DeleteObject(m_bmpItem[i]);		
		DeleteObject(m_bmpItemMask[i]);	
	}
	DeleteObject(m_bmpWait);			
	DeleteObject(m_bmpWaitMask);		
	DeleteObject(m_bmpStart);			
	DeleteObject(m_bmpStartMask);
	DeleteObject(m_bmpOver);			
	DeleteObject(m_bmpOverMask);
	DeleteObject(m_bmpFire);
}

BOOL CPrinceGameDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	// ESCŰ�� ENTERŰ�� �������� ������� �ʰ���
	if(pMsg->message == WM_KEYDOWN || pMsg->wParam == VK_ESCAPE)
    {
        return TRUE;
    }

	return CDialog::PreTranslateMessage(pMsg);
}

UINT CPrinceGameDlg::OnNcHitTest(CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	UINT hit = CDialog::OnNcHitTest(point);

	CRect rectT, rectL, rectR, rectB;
	rectT.left=0;	rectT.top=0;	rectT.right=829;	rectT.bottom=14;
	rectL.left=0;	rectL.top=0;	rectL.right=14;		rectL.bottom=526;
	rectR.left=817;	rectR.top=0;	rectR.right=829;	rectR.bottom=526;
	rectB.left=0;	rectB.top=513;	rectB.right=829;	rectB.bottom=526;
	ScreenToClient(&point);

	// �ܰ� �׵θ� ������ �巡�� �̵�
	if((point.x >= rectT.left && point.x <= rectT.right && 
		point.y >= rectT.top && point.y <= rectT.bottom) ||
		(point.x >= rectL.left && point.x <= rectL.right && 
		point.y >= rectL.top && point.y <= rectL.bottom) ||
		(point.x >= rectR.left && point.x <= rectR.right && 
		point.y >= rectR.top && point.y <= rectR.bottom) ||
		(point.x >= rectB.left && point.x <= rectB.right && 
		point.y >= rectB.top && point.y <= rectB.bottom)) {
		return HTCAPTION;
	}
	else return hit;
	
	// return CDialog::OnNcHitTest(point);
}

void CPrinceGameDlg::InitGame()
{
	int i, j, k;

	/* ----------------------------------- 
	�迭�� �����Ͽ� OFFBOARD ������ �ʹ�
	����� ����� ��ü�� OFFBOARD�� �ϰ� 
	���� ������ Ư¡�� ��� ������ �κи�
	EMPTY�� ���� ����..
	------------------------------------*/

	for(i=0; i<29; i++) {
		for(j=0; j<21; j++)
			m_nGame[i][j] = OFFBOARD;
	}
	
	// ��ü�� OFFBOARD ���� �Ͽ���.. ������ ������
	// EMPTY�� ���� ���⿡�� ���� �ʹ� ����ϴ� ..
	// �ݺ����� �̿��� ��� ���� ����� ������ ??
	// �������� �����Ͽ� �ﰢ�� 2�����ϸ� �ذ��� �ǰڱ��� !!

	k = 15;
	for( i = 2 ; i < 15 ; i++){			//��ﰢ�� ����
		for( j = 16-i ; j < k ; j++){
			if( (k%2 != 0) && (j%2 == 0) ){
				m_nGame[j][i] = EMPTY;
			}
			if( (k%2 == 0) && (j%2 != 0) ){
				m_nGame[j][i] = EMPTY;
			}
		}
		k++;
	}

	k = 13;
	for( i = 18 ; i > 5 ; i--){			//��ﰢ�� ����
		for( j = 32-i ; j > k ; j--){
			if( (k%2 != 0) && (j%2 == 0) ){
				m_nGame[j][i] = EMPTY;
			}
			if( (k%2 == 0) && (j%2 != 0) ){
				m_nGame[j][i] = EMPTY;
			}
		}
		k--;
	}

	// �÷���� ���� ���� ���� �ʱ�ȭ
	switch(m_nPlayer) {

	// 2 Player
	case 2 :

		// RED �� �ʱ�ȭ
		m_nGame[11][15] = HUMAN_RED;	m_nGame[13][15] = HUMAN_RED;
		m_nGame[15][15] = HUMAN_RED;	m_nGame[17][15] = HUMAN_RED;
		m_nGame[12][16] = HUMAN_RED;	m_nGame[14][16] = HUMAN_RED;
		m_nGame[16][16] = HUMAN_RED;	m_nGame[13][17] = HUMAN_RED;
		m_nGame[15][17] = HUMAN_RED;	m_nGame[14][18] = HUMAN_RED;

		// CYAN �� �ʱ�ȭ
		m_nGame[14][2] = HUMAN_CYAN;	m_nGame[13][3] = HUMAN_CYAN;
		m_nGame[15][3] = HUMAN_CYAN;	m_nGame[12][4] = HUMAN_CYAN;
		m_nGame[14][4] = HUMAN_CYAN;	m_nGame[16][4] = HUMAN_CYAN;
		m_nGame[11][5] = HUMAN_CYAN;	m_nGame[13][5] = HUMAN_CYAN;
		m_nGame[15][5] = HUMAN_CYAN;	m_nGame[17][5] = HUMAN_CYAN;

		break;
	
	// 3 Player
	case 3 :

		// RED �� �ʱ�ȭ
		m_nGame[11][15] = HUMAN_RED;	m_nGame[13][15] = HUMAN_RED;
		m_nGame[15][15] = HUMAN_RED;	m_nGame[17][15] = HUMAN_RED;
		m_nGame[12][16] = HUMAN_RED;	m_nGame[14][16] = HUMAN_RED;
		m_nGame[16][16] = HUMAN_RED;	m_nGame[13][17] = HUMAN_RED;
		m_nGame[15][17] = HUMAN_RED;	m_nGame[14][18] = HUMAN_RED;

		// GREEN �� �ʱ�ȭ
		m_nGame[2][6] = HUMAN_GREEN;	m_nGame[4][6] = HUMAN_GREEN;
		m_nGame[6][6] = HUMAN_GREEN;	m_nGame[8][6] = HUMAN_GREEN;
		m_nGame[3][7] = HUMAN_GREEN;	m_nGame[5][7] = HUMAN_GREEN;
		m_nGame[7][7] = HUMAN_GREEN;	m_nGame[4][8] = HUMAN_GREEN;
		m_nGame[6][8] = HUMAN_GREEN;	m_nGame[5][9] = HUMAN_GREEN;

		// BLUE �� �ʱ�ȭ
		m_nGame[20][6] = HUMAN_BLUE;	m_nGame[22][6] = HUMAN_BLUE;
		m_nGame[24][6] = HUMAN_BLUE;	m_nGame[26][6] = HUMAN_BLUE;
		m_nGame[21][7] = HUMAN_BLUE;	m_nGame[23][7] = HUMAN_BLUE;
		m_nGame[25][7] = HUMAN_BLUE;	m_nGame[22][8] = HUMAN_BLUE;
		m_nGame[24][8] = HUMAN_BLUE;	m_nGame[23][9] = HUMAN_BLUE;

		break;

	// 4 Player
	case 4 :

		// RED �� �ʱ�ȭ
		m_nGame[11][15] = HUMAN_RED;	m_nGame[13][15] = HUMAN_RED;
		m_nGame[15][15] = HUMAN_RED;	m_nGame[17][15] = HUMAN_RED;
		m_nGame[12][16] = HUMAN_RED;	m_nGame[14][16] = HUMAN_RED;
		m_nGame[16][16] = HUMAN_RED;	m_nGame[13][17] = HUMAN_RED;
		m_nGame[15][17] = HUMAN_RED;	m_nGame[14][18] = HUMAN_RED;

		// CYAN �� �ʱ�ȭ
		m_nGame[14][2] = HUMAN_CYAN;	m_nGame[13][3] = HUMAN_CYAN;
		m_nGame[15][3] = HUMAN_CYAN;	m_nGame[12][4] = HUMAN_CYAN;
		m_nGame[14][4] = HUMAN_CYAN;	m_nGame[16][4] = HUMAN_CYAN;
		m_nGame[11][5] = HUMAN_CYAN;	m_nGame[13][5] = HUMAN_CYAN;
		m_nGame[15][5] = HUMAN_CYAN;	m_nGame[17][5] = HUMAN_CYAN;

		// GREEN �� �ʱ�ȭ
		m_nGame[2][6] = HUMAN_GREEN;	m_nGame[4][6] = HUMAN_GREEN;
		m_nGame[6][6] = HUMAN_GREEN;	m_nGame[8][6] = HUMAN_GREEN;
		m_nGame[3][7] = HUMAN_GREEN;	m_nGame[5][7] = HUMAN_GREEN;
		m_nGame[7][7] = HUMAN_GREEN;	m_nGame[4][8] = HUMAN_GREEN;
		m_nGame[6][8] = HUMAN_GREEN;	m_nGame[5][9] = HUMAN_GREEN;

		// PINK �� �ʱ�ȭ
		m_nGame[23][11] = HUMAN_PINK;	m_nGame[22][12] = HUMAN_PINK;
		m_nGame[24][12] = HUMAN_PINK;	m_nGame[21][13] = HUMAN_PINK;
		m_nGame[23][13] = HUMAN_PINK;	m_nGame[25][13] = HUMAN_PINK;
		m_nGame[20][14] = HUMAN_PINK;	m_nGame[22][14] = HUMAN_PINK;
		m_nGame[24][14] = HUMAN_PINK;	m_nGame[26][14] = HUMAN_PINK;

		break;

	// 6 Player
	case 6 :

		// RED �� �ʱ�ȭ
		m_nGame[11][15] = HUMAN_RED;	m_nGame[13][15] = HUMAN_RED;
		m_nGame[15][15] = HUMAN_RED;	m_nGame[17][15] = HUMAN_RED;
		m_nGame[12][16] = HUMAN_RED;	m_nGame[14][16] = HUMAN_RED;
		m_nGame[16][16] = HUMAN_RED;	m_nGame[13][17] = HUMAN_RED;
		m_nGame[15][17] = HUMAN_RED;	m_nGame[14][18] = HUMAN_RED;

		// YELLOW �� �ʱ�ȭ
		m_nGame[5][11] = HUMAN_YELLOW;	m_nGame[4][12] = HUMAN_YELLOW;
		m_nGame[6][12] = HUMAN_YELLOW;	m_nGame[3][13] = HUMAN_YELLOW;
		m_nGame[5][13] = HUMAN_YELLOW;	m_nGame[7][13] = HUMAN_YELLOW;
		m_nGame[2][14] = HUMAN_YELLOW;	m_nGame[4][14] = HUMAN_YELLOW;
		m_nGame[6][14] = HUMAN_YELLOW;	m_nGame[8][14] = HUMAN_YELLOW;

		// GREEN �� �ʱ�ȭ
		m_nGame[2][6] = HUMAN_GREEN;	m_nGame[4][6] = HUMAN_GREEN;
		m_nGame[6][6] = HUMAN_GREEN;	m_nGame[8][6] = HUMAN_GREEN;
		m_nGame[3][7] = HUMAN_GREEN;	m_nGame[5][7] = HUMAN_GREEN;
		m_nGame[7][7] = HUMAN_GREEN;	m_nGame[4][8] = HUMAN_GREEN;
		m_nGame[6][8] = HUMAN_GREEN;	m_nGame[5][9] = HUMAN_GREEN;

		// CYAN �� �ʱ�ȭ
		m_nGame[14][2] = HUMAN_CYAN;	m_nGame[13][3] = HUMAN_CYAN;
		m_nGame[15][3] = HUMAN_CYAN;	m_nGame[12][4] = HUMAN_CYAN;
		m_nGame[14][4] = HUMAN_CYAN;	m_nGame[16][4] = HUMAN_CYAN;
		m_nGame[11][5] = HUMAN_CYAN;	m_nGame[13][5] = HUMAN_CYAN;
		m_nGame[15][5] = HUMAN_CYAN;	m_nGame[17][5] = HUMAN_CYAN;

		// BLUE �� �ʱ�ȭ
		m_nGame[20][6] = HUMAN_BLUE;	m_nGame[22][6] = HUMAN_BLUE;
		m_nGame[24][6] = HUMAN_BLUE;	m_nGame[26][6] = HUMAN_BLUE;
		m_nGame[21][7] = HUMAN_BLUE;	m_nGame[23][7] = HUMAN_BLUE;
		m_nGame[25][7] = HUMAN_BLUE;	m_nGame[22][8] = HUMAN_BLUE;
		m_nGame[24][8] = HUMAN_BLUE;	m_nGame[23][9] = HUMAN_BLUE;

		// PINK �� �ʱ�ȭ
		m_nGame[23][11] = HUMAN_PINK;	m_nGame[22][12] = HUMAN_PINK;
		m_nGame[24][12] = HUMAN_PINK;	m_nGame[21][13] = HUMAN_PINK;
		m_nGame[23][13] = HUMAN_PINK;	m_nGame[25][13] = HUMAN_PINK;
		m_nGame[20][14] = HUMAN_PINK;	m_nGame[22][14] = HUMAN_PINK;
		m_nGame[24][14] = HUMAN_PINK;	m_nGame[26][14] = HUMAN_PINK;

		break;

	default :
		AfxMessageBox("���� �ο����� ���� �ʽ��ϴ�.");
		break;
	}

	// �����ο� Ÿ�̸� ����
	SetTimer(REALDRAW, 20, NULL);
	if(m_bCheckWait)
		// ���簣 Ÿ�̸� ����
		SetTimer(WAITTIME, 1000, NULL);
}

int CPrinceGameDlg::GetOtherType(int nType)
{
	switch(m_nPlayer) {
	case 2:
		if( // CYAN ������ RED ��
		m_nGame[14][2] == HUMAN_RED    &&	m_nGame[13][3] == HUMAN_RED &&
		m_nGame[15][3] == HUMAN_RED    &&	m_nGame[12][4] == HUMAN_RED &&
		m_nGame[14][4] == HUMAN_RED    &&	m_nGame[16][4] == HUMAN_RED &&
		m_nGame[11][5] == HUMAN_RED    &&	m_nGame[13][5] == HUMAN_RED &&
		m_nGame[15][5] == HUMAN_RED    &&	m_nGame[17][5] == HUMAN_RED )
			return HUMAN_CYAN;

		else if( // RED ������ CYAN ��
		m_nGame[11][15] == HUMAN_CYAN  &&	m_nGame[13][15] == HUMAN_CYAN &&
		m_nGame[15][15] == HUMAN_CYAN  &&	m_nGame[17][15] == HUMAN_CYAN &&
		m_nGame[12][16] == HUMAN_CYAN  &&	m_nGame[14][16] == HUMAN_CYAN &&
		m_nGame[16][16] == HUMAN_CYAN  &&	m_nGame[13][17] == HUMAN_CYAN &&
		m_nGame[15][17] == HUMAN_CYAN  &&	m_nGame[14][18] == HUMAN_CYAN )
			return HUMAN_RED;
						
		else {
			if(nType == HUMAN_RED)
				return HUMAN_CYAN;
			else if(nType == HUMAN_CYAN)
				return HUMAN_RED;
			break;
		}

	case 3:
		if(nType == HUMAN_RED)
			nType = HUMAN_GREEN;
		else if(nType == HUMAN_GREEN)
			nType = HUMAN_BLUE;
		else if(nType == HUMAN_BLUE)
			nType = HUMAN_RED;

		if(nType == HUMAN_RED) {
			if( // CYAN ������ RED ��
			m_nGame[14][2] == HUMAN_RED    &&	m_nGame[13][3] == HUMAN_RED &&
			m_nGame[15][3] == HUMAN_RED    &&	m_nGame[12][4] == HUMAN_RED &&
			m_nGame[14][4] == HUMAN_RED    &&	m_nGame[16][4] == HUMAN_RED &&
			m_nGame[11][5] == HUMAN_RED    &&	m_nGame[13][5] == HUMAN_RED &&
			m_nGame[15][5] == HUMAN_RED    &&	m_nGame[17][5] == HUMAN_RED &&
				// return HUMAN_GREEN;

			// PINK ������ GREEN ��
			m_nGame[23][11] == HUMAN_GREEN &&	m_nGame[22][12] == HUMAN_GREEN &&
			m_nGame[24][12] == HUMAN_GREEN &&	m_nGame[21][13] == HUMAN_GREEN &&
			m_nGame[23][13] == HUMAN_GREEN &&	m_nGame[25][13] == HUMAN_GREEN &&
			m_nGame[20][14] == HUMAN_GREEN &&	m_nGame[22][14] == HUMAN_GREEN &&
			m_nGame[24][14] == HUMAN_GREEN &&	m_nGame[26][14] == HUMAN_GREEN )
				return HUMAN_BLUE; 

			else if( // CYAN ������ RED ��
			m_nGame[14][2] == HUMAN_RED    &&	m_nGame[13][3] == HUMAN_RED &&
			m_nGame[15][3] == HUMAN_RED    &&	m_nGame[12][4] == HUMAN_RED &&
			m_nGame[14][4] == HUMAN_RED    &&	m_nGame[16][4] == HUMAN_RED &&
			m_nGame[11][5] == HUMAN_RED    &&	m_nGame[13][5] == HUMAN_RED &&
			m_nGame[15][5] == HUMAN_RED    &&	m_nGame[17][5] == HUMAN_RED )
				return HUMAN_GREEN;
		}

		else if(nType == HUMAN_GREEN) {
			if( // PINK ������ GREEN ��
			m_nGame[23][11] == HUMAN_GREEN &&	m_nGame[22][12] == HUMAN_GREEN &&
			m_nGame[24][12] == HUMAN_GREEN &&	m_nGame[21][13] == HUMAN_GREEN &&
			m_nGame[23][13] == HUMAN_GREEN &&	m_nGame[25][13] == HUMAN_GREEN &&
			m_nGame[20][14] == HUMAN_GREEN &&	m_nGame[22][14] == HUMAN_GREEN &&
			m_nGame[24][14] == HUMAN_GREEN &&	m_nGame[26][14] == HUMAN_GREEN &&
				// return HUMAN_BLUE;

			// YELLOW ������ BLUE ��
			m_nGame[5][11] == HUMAN_BLUE   &&	m_nGame[4][12] == HUMAN_BLUE &&
			m_nGame[6][12] == HUMAN_BLUE   &&	m_nGame[3][13] == HUMAN_BLUE &&
			m_nGame[5][13] == HUMAN_BLUE   &&	m_nGame[7][13] == HUMAN_BLUE &&
			m_nGame[2][14] == HUMAN_BLUE   &&	m_nGame[4][14] == HUMAN_BLUE &&
			m_nGame[6][14] == HUMAN_BLUE   &&	m_nGame[8][14] == HUMAN_BLUE )
				return HUMAN_RED;

			else if( // PINK ������ GREEN ��
			m_nGame[23][11] == HUMAN_GREEN &&	m_nGame[22][12] == HUMAN_GREEN &&
			m_nGame[24][12] == HUMAN_GREEN &&	m_nGame[21][13] == HUMAN_GREEN &&
			m_nGame[23][13] == HUMAN_GREEN &&	m_nGame[25][13] == HUMAN_GREEN &&
			m_nGame[20][14] == HUMAN_GREEN &&	m_nGame[22][14] == HUMAN_GREEN &&
			m_nGame[24][14] == HUMAN_GREEN &&	m_nGame[26][14] == HUMAN_GREEN )
				return HUMAN_BLUE;
		}

		else if(nType == HUMAN_BLUE) {
			if( // YELLOW ������ BLUE ��
			m_nGame[5][11] == HUMAN_BLUE   &&	m_nGame[4][12] == HUMAN_BLUE &&
			m_nGame[6][12] == HUMAN_BLUE   &&	m_nGame[3][13] == HUMAN_BLUE &&
			m_nGame[5][13] == HUMAN_BLUE   &&	m_nGame[7][13] == HUMAN_BLUE &&
			m_nGame[2][14] == HUMAN_BLUE   &&	m_nGame[4][14] == HUMAN_BLUE &&
			m_nGame[6][14] == HUMAN_BLUE   &&	m_nGame[8][14] == HUMAN_BLUE &&
				// return HUMAN_RED;
	
			// CYAN ������ RED ��
			m_nGame[14][2] == HUMAN_RED    &&	m_nGame[13][3] == HUMAN_RED &&
			m_nGame[15][3] == HUMAN_RED    &&	m_nGame[12][4] == HUMAN_RED &&
			m_nGame[14][4] == HUMAN_RED    &&	m_nGame[16][4] == HUMAN_RED &&
			m_nGame[11][5] == HUMAN_RED    &&	m_nGame[13][5] == HUMAN_RED &&
			m_nGame[15][5] == HUMAN_RED    &&	m_nGame[17][5] == HUMAN_RED )
				return HUMAN_GREEN;

			else if( // YELLOW ������ BLUE ��
			m_nGame[5][11] == HUMAN_BLUE   &&	m_nGame[4][12] == HUMAN_BLUE &&
			m_nGame[6][12] == HUMAN_BLUE   &&	m_nGame[3][13] == HUMAN_BLUE &&
			m_nGame[5][13] == HUMAN_BLUE   &&	m_nGame[7][13] == HUMAN_BLUE &&
			m_nGame[2][14] == HUMAN_BLUE   &&	m_nGame[4][14] == HUMAN_BLUE &&
			m_nGame[6][14] == HUMAN_BLUE   &&	m_nGame[8][14] == HUMAN_BLUE )
				return HUMAN_RED;
		}

		return nType;
		break;

	case 4:
		if(nType == HUMAN_RED)
			nType = HUMAN_GREEN;
		else if(nType == HUMAN_GREEN)
			nType = HUMAN_CYAN;
		else if(nType == HUMAN_CYAN)
			nType = HUMAN_PINK;
		else if(nType == HUMAN_PINK)
			nType = HUMAN_RED;

		if(nType == HUMAN_RED) {
			if( // CYAN ������ RED ��
			m_nGame[14][2] == HUMAN_RED    &&	m_nGame[13][3] == HUMAN_RED &&
			m_nGame[15][3] == HUMAN_RED    &&	m_nGame[12][4] == HUMAN_RED &&
			m_nGame[14][4] == HUMAN_RED    &&	m_nGame[16][4] == HUMAN_RED &&
			m_nGame[11][5] == HUMAN_RED    &&	m_nGame[13][5] == HUMAN_RED &&
			m_nGame[15][5] == HUMAN_RED    &&	m_nGame[17][5] == HUMAN_RED &&
				// return HUMAN_GREEN;

			// PINK ������ GREEN ��
			m_nGame[23][11] == HUMAN_GREEN &&	m_nGame[22][12] == HUMAN_GREEN &&
			m_nGame[24][12] == HUMAN_GREEN &&	m_nGame[21][13] == HUMAN_GREEN &&
			m_nGame[23][13] == HUMAN_GREEN &&	m_nGame[25][13] == HUMAN_GREEN &&
			m_nGame[20][14] == HUMAN_GREEN &&	m_nGame[22][14] == HUMAN_GREEN &&
			m_nGame[24][14] == HUMAN_GREEN &&	m_nGame[26][14] == HUMAN_GREEN &&
				// return HUMAN_CYAN;

			// RED ������ CYAN ��
			m_nGame[11][15] == HUMAN_CYAN  &&	m_nGame[13][15] == HUMAN_CYAN &&
			m_nGame[15][15] == HUMAN_CYAN  &&	m_nGame[17][15] == HUMAN_CYAN &&
			m_nGame[12][16] == HUMAN_CYAN  &&	m_nGame[14][16] == HUMAN_CYAN &&
			m_nGame[16][16] == HUMAN_CYAN  &&	m_nGame[13][17] == HUMAN_CYAN &&
			m_nGame[15][17] == HUMAN_CYAN  &&	m_nGame[14][18] == HUMAN_CYAN )
				return HUMAN_PINK;

			else if( // CYAN ������ RED ��
			m_nGame[14][2] == HUMAN_RED    &&	m_nGame[13][3] == HUMAN_RED &&
			m_nGame[15][3] == HUMAN_RED    &&	m_nGame[12][4] == HUMAN_RED &&
			m_nGame[14][4] == HUMAN_RED    &&	m_nGame[16][4] == HUMAN_RED &&
			m_nGame[11][5] == HUMAN_RED    &&	m_nGame[13][5] == HUMAN_RED &&
			m_nGame[15][5] == HUMAN_RED    &&	m_nGame[17][5] == HUMAN_RED &&
				// return HUMAN_GREEN;

			// PINK ������ GREEN ��
			m_nGame[23][11] == HUMAN_GREEN &&	m_nGame[22][12] == HUMAN_GREEN &&
			m_nGame[24][12] == HUMAN_GREEN &&	m_nGame[21][13] == HUMAN_GREEN &&
			m_nGame[23][13] == HUMAN_GREEN &&	m_nGame[25][13] == HUMAN_GREEN &&
			m_nGame[20][14] == HUMAN_GREEN &&	m_nGame[22][14] == HUMAN_GREEN &&
			m_nGame[24][14] == HUMAN_GREEN &&	m_nGame[26][14] == HUMAN_GREEN )
				return HUMAN_CYAN;

			else if( // CYAN ������ RED ��
			m_nGame[14][2] == HUMAN_RED    &&	m_nGame[13][3] == HUMAN_RED &&
			m_nGame[15][3] == HUMAN_RED    &&	m_nGame[12][4] == HUMAN_RED &&
			m_nGame[14][4] == HUMAN_RED    &&	m_nGame[16][4] == HUMAN_RED &&
			m_nGame[11][5] == HUMAN_RED    &&	m_nGame[13][5] == HUMAN_RED &&
			m_nGame[15][5] == HUMAN_RED    &&	m_nGame[17][5] == HUMAN_RED )
				return HUMAN_GREEN;
		}

		else if(nType == HUMAN_GREEN) {
			if( // PINK ������ GREEN ��
			m_nGame[23][11] == HUMAN_GREEN &&	m_nGame[22][12] == HUMAN_GREEN &&
			m_nGame[24][12] == HUMAN_GREEN &&	m_nGame[21][13] == HUMAN_GREEN &&
			m_nGame[23][13] == HUMAN_GREEN &&	m_nGame[25][13] == HUMAN_GREEN &&
			m_nGame[20][14] == HUMAN_GREEN &&	m_nGame[22][14] == HUMAN_GREEN &&
			m_nGame[24][14] == HUMAN_GREEN &&	m_nGame[26][14] == HUMAN_GREEN &&
				// return HUMAN_CYAN;

			// RED ������ CYAN ��
			m_nGame[11][15] == HUMAN_CYAN  &&	m_nGame[13][15] == HUMAN_CYAN &&
			m_nGame[15][15] == HUMAN_CYAN  &&	m_nGame[17][15] == HUMAN_CYAN &&
			m_nGame[12][16] == HUMAN_CYAN  &&	m_nGame[14][16] == HUMAN_CYAN &&
			m_nGame[16][16] == HUMAN_CYAN  &&	m_nGame[13][17] == HUMAN_CYAN &&
			m_nGame[15][17] == HUMAN_CYAN  &&	m_nGame[14][18] == HUMAN_CYAN &&
				// return HUMAN_PINK;

			// GREEN ������ PINK ��
			m_nGame[2][6] == HUMAN_PINK    &&	m_nGame[4][6] == HUMAN_PINK &&
			m_nGame[6][6] == HUMAN_PINK    &&	m_nGame[8][6] == HUMAN_PINK &&
			m_nGame[3][7] == HUMAN_PINK    &&	m_nGame[5][7] == HUMAN_PINK &&
			m_nGame[7][7] == HUMAN_PINK    &&	m_nGame[4][8] == HUMAN_PINK &&
			m_nGame[6][8] == HUMAN_PINK    &&	m_nGame[5][9] == HUMAN_PINK )
				return HUMAN_RED;
			
			else if( // PINK ������ GREEN ��
			m_nGame[23][11] == HUMAN_GREEN &&	m_nGame[22][12] == HUMAN_GREEN &&
			m_nGame[24][12] == HUMAN_GREEN &&	m_nGame[21][13] == HUMAN_GREEN &&
			m_nGame[23][13] == HUMAN_GREEN &&	m_nGame[25][13] == HUMAN_GREEN &&
			m_nGame[20][14] == HUMAN_GREEN &&	m_nGame[22][14] == HUMAN_GREEN &&
			m_nGame[24][14] == HUMAN_GREEN &&	m_nGame[26][14] == HUMAN_GREEN &&
				// return HUMAN_CYAN;

			// RED ������ CYAN ��
			m_nGame[11][15] == HUMAN_CYAN  &&	m_nGame[13][15] == HUMAN_CYAN &&
			m_nGame[15][15] == HUMAN_CYAN  &&	m_nGame[17][15] == HUMAN_CYAN &&
			m_nGame[12][16] == HUMAN_CYAN  &&	m_nGame[14][16] == HUMAN_CYAN &&
			m_nGame[16][16] == HUMAN_CYAN  &&	m_nGame[13][17] == HUMAN_CYAN &&
			m_nGame[15][17] == HUMAN_CYAN  &&	m_nGame[14][18] == HUMAN_CYAN )
				return HUMAN_PINK;

			else if( // PINK ������ GREEN ��
			m_nGame[23][11] == HUMAN_GREEN &&	m_nGame[22][12] == HUMAN_GREEN &&
			m_nGame[24][12] == HUMAN_GREEN &&	m_nGame[21][13] == HUMAN_GREEN &&
			m_nGame[23][13] == HUMAN_GREEN &&	m_nGame[25][13] == HUMAN_GREEN &&
			m_nGame[20][14] == HUMAN_GREEN &&	m_nGame[22][14] == HUMAN_GREEN &&
			m_nGame[24][14] == HUMAN_GREEN &&	m_nGame[26][14] == HUMAN_GREEN )
				return HUMAN_CYAN;
		}

		else if(nType == HUMAN_CYAN) {
			if(	// RED ������ CYAN ��
			m_nGame[11][15] == HUMAN_CYAN  &&	m_nGame[13][15] == HUMAN_CYAN &&
			m_nGame[15][15] == HUMAN_CYAN  &&	m_nGame[17][15] == HUMAN_CYAN &&
			m_nGame[12][16] == HUMAN_CYAN  &&	m_nGame[14][16] == HUMAN_CYAN &&
			m_nGame[16][16] == HUMAN_CYAN  &&	m_nGame[13][17] == HUMAN_CYAN &&
			m_nGame[15][17] == HUMAN_CYAN  &&	m_nGame[14][18] == HUMAN_CYAN &&
				// return HUMAN_PINK;

			// GREEN ������ PINK ��
			m_nGame[2][6] == HUMAN_PINK    &&	m_nGame[4][6] == HUMAN_PINK &&
			m_nGame[6][6] == HUMAN_PINK    &&	m_nGame[8][6] == HUMAN_PINK &&
			m_nGame[3][7] == HUMAN_PINK    &&	m_nGame[5][7] == HUMAN_PINK &&
			m_nGame[7][7] == HUMAN_PINK    &&	m_nGame[4][8] == HUMAN_PINK &&
			m_nGame[6][8] == HUMAN_PINK    &&	m_nGame[5][9] == HUMAN_PINK &&
				// return HUMAN_RED;

			// CYAN ������ RED ��
			m_nGame[14][2] == HUMAN_RED    &&	m_nGame[13][3] == HUMAN_RED &&
			m_nGame[15][3] == HUMAN_RED    &&	m_nGame[12][4] == HUMAN_RED &&
			m_nGame[14][4] == HUMAN_RED    &&	m_nGame[16][4] == HUMAN_RED &&
			m_nGame[11][5] == HUMAN_RED    &&	m_nGame[13][5] == HUMAN_RED &&
			m_nGame[15][5] == HUMAN_RED    &&	m_nGame[17][5] == HUMAN_RED )
				return HUMAN_GREEN;

			else if( // RED ������ CYAN ��
			m_nGame[11][15] == HUMAN_CYAN  &&	m_nGame[13][15] == HUMAN_CYAN &&
			m_nGame[15][15] == HUMAN_CYAN  &&	m_nGame[17][15] == HUMAN_CYAN &&
			m_nGame[12][16] == HUMAN_CYAN  &&	m_nGame[14][16] == HUMAN_CYAN &&
			m_nGame[16][16] == HUMAN_CYAN  &&	m_nGame[13][17] == HUMAN_CYAN &&
			m_nGame[15][17] == HUMAN_CYAN  &&	m_nGame[14][18] == HUMAN_CYAN &&
				// return HUMAN_PINK;

			// GREEN ������ PINK ��
			m_nGame[2][6] == HUMAN_PINK    &&	m_nGame[4][6] == HUMAN_PINK &&
			m_nGame[6][6] == HUMAN_PINK    &&	m_nGame[8][6] == HUMAN_PINK &&
			m_nGame[3][7] == HUMAN_PINK    &&	m_nGame[5][7] == HUMAN_PINK &&
			m_nGame[7][7] == HUMAN_PINK    &&	m_nGame[4][8] == HUMAN_PINK &&
			m_nGame[6][8] == HUMAN_PINK    &&	m_nGame[5][9] == HUMAN_PINK )
				return HUMAN_RED;

			else if( // RED ������ CYAN ��
			m_nGame[11][15] == HUMAN_CYAN  &&	m_nGame[13][15] == HUMAN_CYAN &&
			m_nGame[15][15] == HUMAN_CYAN  &&	m_nGame[17][15] == HUMAN_CYAN &&
			m_nGame[12][16] == HUMAN_CYAN  &&	m_nGame[14][16] == HUMAN_CYAN &&
			m_nGame[16][16] == HUMAN_CYAN  &&	m_nGame[13][17] == HUMAN_CYAN &&
			m_nGame[15][17] == HUMAN_CYAN  &&	m_nGame[14][18] == HUMAN_CYAN )
				return HUMAN_PINK;
		}

		else if(nType == HUMAN_PINK) {
			if( // GREEN ������ PINK ��
			m_nGame[2][6] == HUMAN_PINK    &&	m_nGame[4][6] == HUMAN_PINK &&
			m_nGame[6][6] == HUMAN_PINK    &&	m_nGame[8][6] == HUMAN_PINK &&
			m_nGame[3][7] == HUMAN_PINK    &&	m_nGame[5][7] == HUMAN_PINK &&
			m_nGame[7][7] == HUMAN_PINK    &&	m_nGame[4][8] == HUMAN_PINK &&
			m_nGame[6][8] == HUMAN_PINK    &&	m_nGame[5][9] == HUMAN_PINK &&
				// return HUMAN_RED;

			// CYAN ������ RED ��
			m_nGame[14][2] == HUMAN_RED    &&	m_nGame[13][3] == HUMAN_RED &&
			m_nGame[15][3] == HUMAN_RED    &&	m_nGame[12][4] == HUMAN_RED &&
			m_nGame[14][4] == HUMAN_RED    &&	m_nGame[16][4] == HUMAN_RED &&
			m_nGame[11][5] == HUMAN_RED    &&	m_nGame[13][5] == HUMAN_RED &&
			m_nGame[15][5] == HUMAN_RED    &&	m_nGame[17][5] == HUMAN_RED &&
				// return HUMAN_GREEN;

			// PINK ������ GREEN ��
			m_nGame[23][11] == HUMAN_GREEN &&	m_nGame[22][12] == HUMAN_GREEN &&
			m_nGame[24][12] == HUMAN_GREEN &&	m_nGame[21][13] == HUMAN_GREEN &&
			m_nGame[23][13] == HUMAN_GREEN &&	m_nGame[25][13] == HUMAN_GREEN &&
			m_nGame[20][14] == HUMAN_GREEN &&	m_nGame[22][14] == HUMAN_GREEN &&
			m_nGame[24][14] == HUMAN_GREEN &&	m_nGame[26][14] == HUMAN_GREEN )
				return HUMAN_CYAN;

			else if( // GREEN ������ PINK ��
			m_nGame[2][6] == HUMAN_PINK    &&	m_nGame[4][6] == HUMAN_PINK &&
			m_nGame[6][6] == HUMAN_PINK    &&	m_nGame[8][6] == HUMAN_PINK &&
			m_nGame[3][7] == HUMAN_PINK    &&	m_nGame[5][7] == HUMAN_PINK &&
			m_nGame[7][7] == HUMAN_PINK    &&	m_nGame[4][8] == HUMAN_PINK &&
			m_nGame[6][8] == HUMAN_PINK    &&	m_nGame[5][9] == HUMAN_PINK &&
				// return HUMAN_RED;

			// CYAN ������ RED ��
			m_nGame[14][2] == HUMAN_RED    &&	m_nGame[13][3] == HUMAN_RED &&
			m_nGame[15][3] == HUMAN_RED    &&	m_nGame[12][4] == HUMAN_RED &&
			m_nGame[14][4] == HUMAN_RED    &&	m_nGame[16][4] == HUMAN_RED &&
			m_nGame[11][5] == HUMAN_RED    &&	m_nGame[13][5] == HUMAN_RED &&
			m_nGame[15][5] == HUMAN_RED    &&	m_nGame[17][5] == HUMAN_RED )
				return HUMAN_GREEN;

			else if( // GREEN ������ PINK ��
			m_nGame[2][6] == HUMAN_PINK    &&	m_nGame[4][6] == HUMAN_PINK &&
			m_nGame[6][6] == HUMAN_PINK    &&	m_nGame[8][6] == HUMAN_PINK &&
			m_nGame[3][7] == HUMAN_PINK    &&	m_nGame[5][7] == HUMAN_PINK &&
			m_nGame[7][7] == HUMAN_PINK    &&	m_nGame[4][8] == HUMAN_PINK &&
			m_nGame[6][8] == HUMAN_PINK    &&	m_nGame[5][9] == HUMAN_PINK )
				return HUMAN_RED;
		}

		return nType;
		break;

	case 6:
		if(nType == HUMAN_RED)
			nType = HUMAN_YELLOW;
		else if(nType == HUMAN_YELLOW)
			nType = HUMAN_GREEN;
		else if(nType == HUMAN_GREEN)
			nType = HUMAN_CYAN;
		else if(nType == HUMAN_CYAN)
			nType = HUMAN_BLUE;
		else if(nType == HUMAN_BLUE)
			nType = HUMAN_PINK;
		else if(nType == HUMAN_PINK)
			nType = HUMAN_RED;

		// ������ ���� �ִ��� üũ
		if(nType == HUMAN_RED) {
			if( // CYAN ������ RED ��
			m_nGame[14][2] == HUMAN_RED    &&	m_nGame[13][3] == HUMAN_RED &&
			m_nGame[15][3] == HUMAN_RED    &&	m_nGame[12][4] == HUMAN_RED &&
			m_nGame[14][4] == HUMAN_RED    &&	m_nGame[16][4] == HUMAN_RED &&
			m_nGame[11][5] == HUMAN_RED    &&	m_nGame[13][5] == HUMAN_RED &&
			m_nGame[15][5] == HUMAN_RED    &&	m_nGame[17][5] == HUMAN_RED &&
				// return HUMAN_YELLOW;

			// BLUE ������ YELLOW ��
			m_nGame[20][6] == HUMAN_YELLOW &&	m_nGame[22][6] == HUMAN_YELLOW &&
			m_nGame[24][6] == HUMAN_YELLOW &&	m_nGame[26][6] == HUMAN_YELLOW &&
			m_nGame[21][7] == HUMAN_YELLOW &&	m_nGame[23][7] == HUMAN_YELLOW &&
			m_nGame[25][7] == HUMAN_YELLOW &&	m_nGame[22][8] == HUMAN_YELLOW &&
			m_nGame[24][8] == HUMAN_YELLOW &&	m_nGame[23][9] == HUMAN_YELLOW &&
				// return HUMAN_GREEN;

			// PINK ������ GREEN ��
			m_nGame[23][11] == HUMAN_GREEN &&	m_nGame[22][12] == HUMAN_GREEN &&
			m_nGame[24][12] == HUMAN_GREEN &&	m_nGame[21][13] == HUMAN_GREEN &&
			m_nGame[23][13] == HUMAN_GREEN &&	m_nGame[25][13] == HUMAN_GREEN &&
			m_nGame[20][14] == HUMAN_GREEN &&	m_nGame[22][14] == HUMAN_GREEN &&
			m_nGame[24][14] == HUMAN_GREEN &&	m_nGame[26][14] == HUMAN_GREEN &&
				// return HUMAN_CYAN;

			// RED ������ CYAN ��
			m_nGame[11][15] == HUMAN_CYAN  &&	m_nGame[13][15] == HUMAN_CYAN &&
			m_nGame[15][15] == HUMAN_CYAN  &&	m_nGame[17][15] == HUMAN_CYAN &&
			m_nGame[12][16] == HUMAN_CYAN  &&	m_nGame[14][16] == HUMAN_CYAN &&
			m_nGame[16][16] == HUMAN_CYAN  &&	m_nGame[13][17] == HUMAN_CYAN &&
			m_nGame[15][17] == HUMAN_CYAN  &&	m_nGame[14][18] == HUMAN_CYAN &&
				// return HUMAN_BLUE;

			// YELLOW ������ BLUE ��
			m_nGame[5][11] == HUMAN_BLUE   &&	m_nGame[4][12] == HUMAN_BLUE &&
			m_nGame[6][12] == HUMAN_BLUE   &&	m_nGame[3][13] == HUMAN_BLUE &&
			m_nGame[5][13] == HUMAN_BLUE   &&	m_nGame[7][13] == HUMAN_BLUE &&
			m_nGame[2][14] == HUMAN_BLUE   &&	m_nGame[4][14] == HUMAN_BLUE &&
			m_nGame[6][14] == HUMAN_BLUE   &&	m_nGame[8][14] == HUMAN_BLUE )
				return HUMAN_PINK;	
			
			else if( // CYAN ������ RED ��
			m_nGame[14][2] == HUMAN_RED    &&	m_nGame[13][3] == HUMAN_RED &&
			m_nGame[15][3] == HUMAN_RED    &&	m_nGame[12][4] == HUMAN_RED &&
			m_nGame[14][4] == HUMAN_RED    &&	m_nGame[16][4] == HUMAN_RED &&
			m_nGame[11][5] == HUMAN_RED    &&	m_nGame[13][5] == HUMAN_RED &&
			m_nGame[15][5] == HUMAN_RED    &&	m_nGame[17][5] == HUMAN_RED &&
				// return HUMAN_YELLOW;

			// BLUE ������ YELLOW ��
			m_nGame[20][6] == HUMAN_YELLOW &&	m_nGame[22][6] == HUMAN_YELLOW &&
			m_nGame[24][6] == HUMAN_YELLOW &&	m_nGame[26][6] == HUMAN_YELLOW &&
			m_nGame[21][7] == HUMAN_YELLOW &&	m_nGame[23][7] == HUMAN_YELLOW &&
			m_nGame[25][7] == HUMAN_YELLOW &&	m_nGame[22][8] == HUMAN_YELLOW &&
			m_nGame[24][8] == HUMAN_YELLOW &&	m_nGame[23][9] == HUMAN_YELLOW &&
				// return HUMAN_GREEN;

			// PINK ������ GREEN ��
			m_nGame[23][11] == HUMAN_GREEN &&	m_nGame[22][12] == HUMAN_GREEN &&
			m_nGame[24][12] == HUMAN_GREEN &&	m_nGame[21][13] == HUMAN_GREEN &&
			m_nGame[23][13] == HUMAN_GREEN &&	m_nGame[25][13] == HUMAN_GREEN &&
			m_nGame[20][14] == HUMAN_GREEN &&	m_nGame[22][14] == HUMAN_GREEN &&
			m_nGame[24][14] == HUMAN_GREEN &&	m_nGame[26][14] == HUMAN_GREEN &&
				// return HUMAN_CYAN;

			// RED ������ CYAN ��
			m_nGame[11][15] == HUMAN_CYAN  &&	m_nGame[13][15] == HUMAN_CYAN &&
			m_nGame[15][15] == HUMAN_CYAN  &&	m_nGame[17][15] == HUMAN_CYAN &&
			m_nGame[12][16] == HUMAN_CYAN  &&	m_nGame[14][16] == HUMAN_CYAN &&
			m_nGame[16][16] == HUMAN_CYAN  &&	m_nGame[13][17] == HUMAN_CYAN &&
			m_nGame[15][17] == HUMAN_CYAN  &&	m_nGame[14][18] == HUMAN_CYAN )
				return HUMAN_BLUE;

			else if( // CYAN ������ RED ��
			m_nGame[14][2] == HUMAN_RED    &&	m_nGame[13][3] == HUMAN_RED &&
			m_nGame[15][3] == HUMAN_RED    &&	m_nGame[12][4] == HUMAN_RED &&
			m_nGame[14][4] == HUMAN_RED    &&	m_nGame[16][4] == HUMAN_RED &&
			m_nGame[11][5] == HUMAN_RED    &&	m_nGame[13][5] == HUMAN_RED &&
			m_nGame[15][5] == HUMAN_RED    &&	m_nGame[17][5] == HUMAN_RED &&
				// return HUMAN_YELLOW;

			// BLUE ������ YELLOW ��
			m_nGame[20][6] == HUMAN_YELLOW &&	m_nGame[22][6] == HUMAN_YELLOW &&
			m_nGame[24][6] == HUMAN_YELLOW &&	m_nGame[26][6] == HUMAN_YELLOW &&
			m_nGame[21][7] == HUMAN_YELLOW &&	m_nGame[23][7] == HUMAN_YELLOW &&
			m_nGame[25][7] == HUMAN_YELLOW &&	m_nGame[22][8] == HUMAN_YELLOW &&
			m_nGame[24][8] == HUMAN_YELLOW &&	m_nGame[23][9] == HUMAN_YELLOW &&
				// return HUMAN_GREEN;

			// PINK ������ GREEN ��
			m_nGame[23][11] == HUMAN_GREEN &&	m_nGame[22][12] == HUMAN_GREEN &&
			m_nGame[24][12] == HUMAN_GREEN &&	m_nGame[21][13] == HUMAN_GREEN &&
			m_nGame[23][13] == HUMAN_GREEN &&	m_nGame[25][13] == HUMAN_GREEN &&
			m_nGame[20][14] == HUMAN_GREEN &&	m_nGame[22][14] == HUMAN_GREEN &&
			m_nGame[24][14] == HUMAN_GREEN &&	m_nGame[26][14] == HUMAN_GREEN )
				return HUMAN_CYAN;

			else if( // CYAN ������ RED ��
			m_nGame[14][2] == HUMAN_RED    &&	m_nGame[13][3] == HUMAN_RED &&
			m_nGame[15][3] == HUMAN_RED    &&	m_nGame[12][4] == HUMAN_RED &&
			m_nGame[14][4] == HUMAN_RED    &&	m_nGame[16][4] == HUMAN_RED &&
			m_nGame[11][5] == HUMAN_RED    &&	m_nGame[13][5] == HUMAN_RED &&
			m_nGame[15][5] == HUMAN_RED    &&	m_nGame[17][5] == HUMAN_RED &&
				// return HUMAN_YELLOW;

			// BLUE ������ YELLOW ��
			m_nGame[20][6] == HUMAN_YELLOW &&	m_nGame[22][6] == HUMAN_YELLOW &&
			m_nGame[24][6] == HUMAN_YELLOW &&	m_nGame[26][6] == HUMAN_YELLOW &&
			m_nGame[21][7] == HUMAN_YELLOW &&	m_nGame[23][7] == HUMAN_YELLOW &&
			m_nGame[25][7] == HUMAN_YELLOW &&	m_nGame[22][8] == HUMAN_YELLOW &&
			m_nGame[24][8] == HUMAN_YELLOW &&	m_nGame[23][9] == HUMAN_YELLOW )
				return HUMAN_GREEN;

			else if( // CYAN ������ RED ��
			m_nGame[14][2] == HUMAN_RED    &&	m_nGame[13][3] == HUMAN_RED &&
			m_nGame[15][3] == HUMAN_RED    &&	m_nGame[12][4] == HUMAN_RED &&
			m_nGame[14][4] == HUMAN_RED    &&	m_nGame[16][4] == HUMAN_RED &&
			m_nGame[11][5] == HUMAN_RED    &&	m_nGame[13][5] == HUMAN_RED &&
			m_nGame[15][5] == HUMAN_RED    &&	m_nGame[17][5] == HUMAN_RED )
				return HUMAN_YELLOW;
		}

		else if(nType == HUMAN_YELLOW) {
			if(	// BLUE ������ YELLOW ��
			m_nGame[20][6] == HUMAN_YELLOW &&	m_nGame[22][6] == HUMAN_YELLOW &&
			m_nGame[24][6] == HUMAN_YELLOW &&	m_nGame[26][6] == HUMAN_YELLOW &&
			m_nGame[21][7] == HUMAN_YELLOW &&	m_nGame[23][7] == HUMAN_YELLOW &&
			m_nGame[25][7] == HUMAN_YELLOW &&	m_nGame[22][8] == HUMAN_YELLOW &&
			m_nGame[24][8] == HUMAN_YELLOW &&	m_nGame[23][9] == HUMAN_YELLOW &&
				// return HUMAN_GREEN;

			// PINK ������ GREEN ��
			m_nGame[23][11] == HUMAN_GREEN &&	m_nGame[22][12] == HUMAN_GREEN &&
			m_nGame[24][12] == HUMAN_GREEN &&	m_nGame[21][13] == HUMAN_GREEN &&
			m_nGame[23][13] == HUMAN_GREEN &&	m_nGame[25][13] == HUMAN_GREEN &&
			m_nGame[20][14] == HUMAN_GREEN &&	m_nGame[22][14] == HUMAN_GREEN &&
			m_nGame[24][14] == HUMAN_GREEN &&	m_nGame[26][14] == HUMAN_GREEN &&
				// return HUMAN_CYAN;

			// RED ������ CYAN ��
			m_nGame[11][15] == HUMAN_CYAN  &&	m_nGame[13][15] == HUMAN_CYAN &&
			m_nGame[15][15] == HUMAN_CYAN  &&	m_nGame[17][15] == HUMAN_CYAN &&
			m_nGame[12][16] == HUMAN_CYAN  &&	m_nGame[14][16] == HUMAN_CYAN &&
			m_nGame[16][16] == HUMAN_CYAN  &&	m_nGame[13][17] == HUMAN_CYAN &&
			m_nGame[15][17] == HUMAN_CYAN  &&	m_nGame[14][18] == HUMAN_CYAN &&
				// return HUMAN_BLUE;

			// YELLOW ������ BLUE ��
			m_nGame[5][11] == HUMAN_BLUE   &&	m_nGame[4][12] == HUMAN_BLUE &&
			m_nGame[6][12] == HUMAN_BLUE   &&	m_nGame[3][13] == HUMAN_BLUE &&
			m_nGame[5][13] == HUMAN_BLUE   &&	m_nGame[7][13] == HUMAN_BLUE &&
			m_nGame[2][14] == HUMAN_BLUE   &&	m_nGame[4][14] == HUMAN_BLUE &&
			m_nGame[6][14] == HUMAN_BLUE   &&	m_nGame[8][14] == HUMAN_BLUE &&
				// return HUMAN_PINK;

			// GREEN ������ PINK ��
			m_nGame[2][6] == HUMAN_PINK    &&	m_nGame[4][6] == HUMAN_PINK &&
			m_nGame[6][6] == HUMAN_PINK    &&	m_nGame[8][6] == HUMAN_PINK &&
			m_nGame[3][7] == HUMAN_PINK    &&	m_nGame[5][7] == HUMAN_PINK &&
			m_nGame[7][7] == HUMAN_PINK    &&	m_nGame[4][8] == HUMAN_PINK &&
			m_nGame[6][8] == HUMAN_PINK    &&	m_nGame[5][9] == HUMAN_PINK )
				return HUMAN_RED;

			else if( // BLUE ������ YELLOW ��
			m_nGame[20][6] == HUMAN_YELLOW &&	m_nGame[22][6] == HUMAN_YELLOW &&
			m_nGame[24][6] == HUMAN_YELLOW &&	m_nGame[26][6] == HUMAN_YELLOW &&
			m_nGame[21][7] == HUMAN_YELLOW &&	m_nGame[23][7] == HUMAN_YELLOW &&
			m_nGame[25][7] == HUMAN_YELLOW &&	m_nGame[22][8] == HUMAN_YELLOW &&
			m_nGame[24][8] == HUMAN_YELLOW &&	m_nGame[23][9] == HUMAN_YELLOW &&
				// return HUMAN_GREEN;

			// PINK ������ GREEN ��
			m_nGame[23][11] == HUMAN_GREEN &&	m_nGame[22][12] == HUMAN_GREEN &&
			m_nGame[24][12] == HUMAN_GREEN &&	m_nGame[21][13] == HUMAN_GREEN &&
			m_nGame[23][13] == HUMAN_GREEN &&	m_nGame[25][13] == HUMAN_GREEN &&
			m_nGame[20][14] == HUMAN_GREEN &&	m_nGame[22][14] == HUMAN_GREEN &&
			m_nGame[24][14] == HUMAN_GREEN &&	m_nGame[26][14] == HUMAN_GREEN &&
				// return HUMAN_CYAN;

			// RED ������ CYAN ��
			m_nGame[11][15] == HUMAN_CYAN  &&	m_nGame[13][15] == HUMAN_CYAN &&
			m_nGame[15][15] == HUMAN_CYAN  &&	m_nGame[17][15] == HUMAN_CYAN &&
			m_nGame[12][16] == HUMAN_CYAN  &&	m_nGame[14][16] == HUMAN_CYAN &&
			m_nGame[16][16] == HUMAN_CYAN  &&	m_nGame[13][17] == HUMAN_CYAN &&
			m_nGame[15][17] == HUMAN_CYAN  &&	m_nGame[14][18] == HUMAN_CYAN &&
				// return HUMAN_BLUE;

			// YELLOW ������ BLUE ��
			m_nGame[5][11] == HUMAN_BLUE   &&	m_nGame[4][12] == HUMAN_BLUE &&
			m_nGame[6][12] == HUMAN_BLUE   &&	m_nGame[3][13] == HUMAN_BLUE &&
			m_nGame[5][13] == HUMAN_BLUE   &&	m_nGame[7][13] == HUMAN_BLUE &&
			m_nGame[2][14] == HUMAN_BLUE   &&	m_nGame[4][14] == HUMAN_BLUE &&
			m_nGame[6][14] == HUMAN_BLUE   &&	m_nGame[8][14] == HUMAN_BLUE )
				return HUMAN_PINK;

			else if( // BLUE ������ YELLOW ��
			m_nGame[20][6] == HUMAN_YELLOW &&	m_nGame[22][6] == HUMAN_YELLOW &&
			m_nGame[24][6] == HUMAN_YELLOW &&	m_nGame[26][6] == HUMAN_YELLOW &&
			m_nGame[21][7] == HUMAN_YELLOW &&	m_nGame[23][7] == HUMAN_YELLOW &&
			m_nGame[25][7] == HUMAN_YELLOW &&	m_nGame[22][8] == HUMAN_YELLOW &&
			m_nGame[24][8] == HUMAN_YELLOW &&	m_nGame[23][9] == HUMAN_YELLOW &&
				// return HUMAN_GREEN;

			// PINK ������ GREEN ��
			m_nGame[23][11] == HUMAN_GREEN &&	m_nGame[22][12] == HUMAN_GREEN &&
			m_nGame[24][12] == HUMAN_GREEN &&	m_nGame[21][13] == HUMAN_GREEN &&
			m_nGame[23][13] == HUMAN_GREEN &&	m_nGame[25][13] == HUMAN_GREEN &&
			m_nGame[20][14] == HUMAN_GREEN &&	m_nGame[22][14] == HUMAN_GREEN &&
			m_nGame[24][14] == HUMAN_GREEN &&	m_nGame[26][14] == HUMAN_GREEN &&
				// return HUMAN_CYAN;

			// RED ������ CYAN ��
			m_nGame[11][15] == HUMAN_CYAN  &&	m_nGame[13][15] == HUMAN_CYAN &&
			m_nGame[15][15] == HUMAN_CYAN  &&	m_nGame[17][15] == HUMAN_CYAN &&
			m_nGame[12][16] == HUMAN_CYAN  &&	m_nGame[14][16] == HUMAN_CYAN &&
			m_nGame[16][16] == HUMAN_CYAN  &&	m_nGame[13][17] == HUMAN_CYAN &&
			m_nGame[15][17] == HUMAN_CYAN  &&	m_nGame[14][18] == HUMAN_CYAN )
				return HUMAN_BLUE;

			else if( // BLUE ������ YELLOW ��
			m_nGame[20][6] == HUMAN_YELLOW &&	m_nGame[22][6] == HUMAN_YELLOW &&
			m_nGame[24][6] == HUMAN_YELLOW &&	m_nGame[26][6] == HUMAN_YELLOW &&
			m_nGame[21][7] == HUMAN_YELLOW &&	m_nGame[23][7] == HUMAN_YELLOW &&
			m_nGame[25][7] == HUMAN_YELLOW &&	m_nGame[22][8] == HUMAN_YELLOW &&
			m_nGame[24][8] == HUMAN_YELLOW &&	m_nGame[23][9] == HUMAN_YELLOW &&
				// return HUMAN_GREEN;

			// PINK ������ GREEN ��
			m_nGame[23][11] == HUMAN_GREEN &&	m_nGame[22][12] == HUMAN_GREEN &&
			m_nGame[24][12] == HUMAN_GREEN &&	m_nGame[21][13] == HUMAN_GREEN &&
			m_nGame[23][13] == HUMAN_GREEN &&	m_nGame[25][13] == HUMAN_GREEN &&
			m_nGame[20][14] == HUMAN_GREEN &&	m_nGame[22][14] == HUMAN_GREEN &&
			m_nGame[24][14] == HUMAN_GREEN &&	m_nGame[26][14] == HUMAN_GREEN )
				return HUMAN_CYAN;

			else if( // BLUE ������ YELLOW ��
			m_nGame[20][6] == HUMAN_YELLOW &&	m_nGame[22][6] == HUMAN_YELLOW &&
			m_nGame[24][6] == HUMAN_YELLOW &&	m_nGame[26][6] == HUMAN_YELLOW &&
			m_nGame[21][7] == HUMAN_YELLOW &&	m_nGame[23][7] == HUMAN_YELLOW &&
			m_nGame[25][7] == HUMAN_YELLOW &&	m_nGame[22][8] == HUMAN_YELLOW &&
			m_nGame[24][8] == HUMAN_YELLOW &&	m_nGame[23][9] == HUMAN_YELLOW )
				return HUMAN_GREEN;
		}

		else if(nType == HUMAN_GREEN) {
			if( // PINK ������ GREEN ��
			m_nGame[23][11] == HUMAN_GREEN &&	m_nGame[22][12] == HUMAN_GREEN &&
			m_nGame[24][12] == HUMAN_GREEN &&	m_nGame[21][13] == HUMAN_GREEN &&
			m_nGame[23][13] == HUMAN_GREEN &&	m_nGame[25][13] == HUMAN_GREEN &&
			m_nGame[20][14] == HUMAN_GREEN &&	m_nGame[22][14] == HUMAN_GREEN &&
			m_nGame[24][14] == HUMAN_GREEN &&	m_nGame[26][14] == HUMAN_GREEN &&
				// return HUMAN_CYAN;

			// RED ������ CYAN ��
			m_nGame[11][15] == HUMAN_CYAN  &&	m_nGame[13][15] == HUMAN_CYAN &&
			m_nGame[15][15] == HUMAN_CYAN  &&	m_nGame[17][15] == HUMAN_CYAN &&
			m_nGame[12][16] == HUMAN_CYAN  &&	m_nGame[14][16] == HUMAN_CYAN &&
			m_nGame[16][16] == HUMAN_CYAN  &&	m_nGame[13][17] == HUMAN_CYAN &&
			m_nGame[15][17] == HUMAN_CYAN  &&	m_nGame[14][18] == HUMAN_CYAN &&
				// return HUMAN_BLUE;

			// YELLOW ������ BLUE ��
			m_nGame[5][11] == HUMAN_BLUE   &&	m_nGame[4][12] == HUMAN_BLUE &&
			m_nGame[6][12] == HUMAN_BLUE   &&	m_nGame[3][13] == HUMAN_BLUE &&
			m_nGame[5][13] == HUMAN_BLUE   &&	m_nGame[7][13] == HUMAN_BLUE &&
			m_nGame[2][14] == HUMAN_BLUE   &&	m_nGame[4][14] == HUMAN_BLUE &&
			m_nGame[6][14] == HUMAN_BLUE   &&	m_nGame[8][14] == HUMAN_BLUE &&
				// return HUMAN_PINK;

			// GREEN ������ PINK ��
			m_nGame[2][6] == HUMAN_PINK    &&	m_nGame[4][6] == HUMAN_PINK &&
			m_nGame[6][6] == HUMAN_PINK    &&	m_nGame[8][6] == HUMAN_PINK &&
			m_nGame[3][7] == HUMAN_PINK    &&	m_nGame[5][7] == HUMAN_PINK &&
			m_nGame[7][7] == HUMAN_PINK    &&	m_nGame[4][8] == HUMAN_PINK &&
			m_nGame[6][8] == HUMAN_PINK    &&	m_nGame[5][9] == HUMAN_PINK &&
				// return HUMAN_RED;

			// CYAN ������ RED ��
			m_nGame[14][2] == HUMAN_RED    &&	m_nGame[13][3] == HUMAN_RED &&
			m_nGame[15][3] == HUMAN_RED    &&	m_nGame[12][4] == HUMAN_RED &&
			m_nGame[14][4] == HUMAN_RED    &&	m_nGame[16][4] == HUMAN_RED &&
			m_nGame[11][5] == HUMAN_RED    &&	m_nGame[13][5] == HUMAN_RED &&
			m_nGame[15][5] == HUMAN_RED    &&	m_nGame[17][5] == HUMAN_RED )
				return HUMAN_YELLOW;

			else if( // PINK ������ GREEN ��
			m_nGame[23][11] == HUMAN_GREEN &&	m_nGame[22][12] == HUMAN_GREEN &&
			m_nGame[24][12] == HUMAN_GREEN &&	m_nGame[21][13] == HUMAN_GREEN &&
			m_nGame[23][13] == HUMAN_GREEN &&	m_nGame[25][13] == HUMAN_GREEN &&
			m_nGame[20][14] == HUMAN_GREEN &&	m_nGame[22][14] == HUMAN_GREEN &&
			m_nGame[24][14] == HUMAN_GREEN &&	m_nGame[26][14] == HUMAN_GREEN &&
				// return HUMAN_CYAN;

			// RED ������ CYAN ��
			m_nGame[11][15] == HUMAN_CYAN  &&	m_nGame[13][15] == HUMAN_CYAN &&
			m_nGame[15][15] == HUMAN_CYAN  &&	m_nGame[17][15] == HUMAN_CYAN &&
			m_nGame[12][16] == HUMAN_CYAN  &&	m_nGame[14][16] == HUMAN_CYAN &&
			m_nGame[16][16] == HUMAN_CYAN  &&	m_nGame[13][17] == HUMAN_CYAN &&
			m_nGame[15][17] == HUMAN_CYAN  &&	m_nGame[14][18] == HUMAN_CYAN &&
				// return HUMAN_BLUE;

			// YELLOW ������ BLUE ��
			m_nGame[5][11] == HUMAN_BLUE   &&	m_nGame[4][12] == HUMAN_BLUE &&
			m_nGame[6][12] == HUMAN_BLUE   &&	m_nGame[3][13] == HUMAN_BLUE &&
			m_nGame[5][13] == HUMAN_BLUE   &&	m_nGame[7][13] == HUMAN_BLUE &&
			m_nGame[2][14] == HUMAN_BLUE   &&	m_nGame[4][14] == HUMAN_BLUE &&
			m_nGame[6][14] == HUMAN_BLUE   &&	m_nGame[8][14] == HUMAN_BLUE &&
				// return HUMAN_PINK;

			// GREEN ������ PINK ��
			m_nGame[2][6] == HUMAN_PINK    &&	m_nGame[4][6] == HUMAN_PINK &&
			m_nGame[6][6] == HUMAN_PINK    &&	m_nGame[8][6] == HUMAN_PINK &&
			m_nGame[3][7] == HUMAN_PINK    &&	m_nGame[5][7] == HUMAN_PINK &&
			m_nGame[7][7] == HUMAN_PINK    &&	m_nGame[4][8] == HUMAN_PINK &&
			m_nGame[6][8] == HUMAN_PINK    &&	m_nGame[5][9] == HUMAN_PINK )
				return HUMAN_RED;

			else if( // PINK ������ GREEN ��
			m_nGame[23][11] == HUMAN_GREEN &&	m_nGame[22][12] == HUMAN_GREEN &&
			m_nGame[24][12] == HUMAN_GREEN &&	m_nGame[21][13] == HUMAN_GREEN &&
			m_nGame[23][13] == HUMAN_GREEN &&	m_nGame[25][13] == HUMAN_GREEN &&
			m_nGame[20][14] == HUMAN_GREEN &&	m_nGame[22][14] == HUMAN_GREEN &&
			m_nGame[24][14] == HUMAN_GREEN &&	m_nGame[26][14] == HUMAN_GREEN &&
				// return HUMAN_CYAN;

			// RED ������ CYAN ��
			m_nGame[11][15] == HUMAN_CYAN  &&	m_nGame[13][15] == HUMAN_CYAN &&
			m_nGame[15][15] == HUMAN_CYAN  &&	m_nGame[17][15] == HUMAN_CYAN &&
			m_nGame[12][16] == HUMAN_CYAN  &&	m_nGame[14][16] == HUMAN_CYAN &&
			m_nGame[16][16] == HUMAN_CYAN  &&	m_nGame[13][17] == HUMAN_CYAN &&
			m_nGame[15][17] == HUMAN_CYAN  &&	m_nGame[14][18] == HUMAN_CYAN &&
				// return HUMAN_BLUE;

			// YELLOW ������ BLUE ��
			m_nGame[5][11] == HUMAN_BLUE   &&	m_nGame[4][12] == HUMAN_BLUE &&
			m_nGame[6][12] == HUMAN_BLUE   &&	m_nGame[3][13] == HUMAN_BLUE &&
			m_nGame[5][13] == HUMAN_BLUE   &&	m_nGame[7][13] == HUMAN_BLUE &&
			m_nGame[2][14] == HUMAN_BLUE   &&	m_nGame[4][14] == HUMAN_BLUE &&
			m_nGame[6][14] == HUMAN_BLUE   &&	m_nGame[8][14] == HUMAN_BLUE )
				return HUMAN_PINK;

			else if( // PINK ������ GREEN ��
			m_nGame[23][11] == HUMAN_GREEN &&	m_nGame[22][12] == HUMAN_GREEN &&
			m_nGame[24][12] == HUMAN_GREEN &&	m_nGame[21][13] == HUMAN_GREEN &&
			m_nGame[23][13] == HUMAN_GREEN &&	m_nGame[25][13] == HUMAN_GREEN &&
			m_nGame[20][14] == HUMAN_GREEN &&	m_nGame[22][14] == HUMAN_GREEN &&
			m_nGame[24][14] == HUMAN_GREEN &&	m_nGame[26][14] == HUMAN_GREEN &&
				// return HUMAN_CYAN;

			// RED ������ CYAN ��
			m_nGame[11][15] == HUMAN_CYAN  &&	m_nGame[13][15] == HUMAN_CYAN &&
			m_nGame[15][15] == HUMAN_CYAN  &&	m_nGame[17][15] == HUMAN_CYAN &&
			m_nGame[12][16] == HUMAN_CYAN  &&	m_nGame[14][16] == HUMAN_CYAN &&
			m_nGame[16][16] == HUMAN_CYAN  &&	m_nGame[13][17] == HUMAN_CYAN &&
			m_nGame[15][17] == HUMAN_CYAN  &&	m_nGame[14][18] == HUMAN_CYAN )
				return HUMAN_BLUE;

			else if( // PINK ������ GREEN ��
			m_nGame[23][11] == HUMAN_GREEN &&	m_nGame[22][12] == HUMAN_GREEN &&
			m_nGame[24][12] == HUMAN_GREEN &&	m_nGame[21][13] == HUMAN_GREEN &&
			m_nGame[23][13] == HUMAN_GREEN &&	m_nGame[25][13] == HUMAN_GREEN &&
			m_nGame[20][14] == HUMAN_GREEN &&	m_nGame[22][14] == HUMAN_GREEN &&
			m_nGame[24][14] == HUMAN_GREEN &&	m_nGame[26][14] == HUMAN_GREEN )
				return HUMAN_CYAN;
		}

		else if(nType == HUMAN_CYAN) {
			if( // RED ������ CYAN ��
			m_nGame[11][15] == HUMAN_CYAN  &&	m_nGame[13][15] == HUMAN_CYAN &&
			m_nGame[15][15] == HUMAN_CYAN  &&	m_nGame[17][15] == HUMAN_CYAN &&
			m_nGame[12][16] == HUMAN_CYAN  &&	m_nGame[14][16] == HUMAN_CYAN &&
			m_nGame[16][16] == HUMAN_CYAN  &&	m_nGame[13][17] == HUMAN_CYAN &&
			m_nGame[15][17] == HUMAN_CYAN  &&	m_nGame[14][18] == HUMAN_CYAN &&
				// return HUMAN_BLUE;

			// YELLOW ������ BLUE ��
			m_nGame[5][11] == HUMAN_BLUE   &&	m_nGame[4][12] == HUMAN_BLUE &&
			m_nGame[6][12] == HUMAN_BLUE   &&	m_nGame[3][13] == HUMAN_BLUE &&
			m_nGame[5][13] == HUMAN_BLUE   &&	m_nGame[7][13] == HUMAN_BLUE &&
			m_nGame[2][14] == HUMAN_BLUE   &&	m_nGame[4][14] == HUMAN_BLUE &&
			m_nGame[6][14] == HUMAN_BLUE   &&	m_nGame[8][14] == HUMAN_BLUE &&
				// return HUMAN_PINK;

			// GREEN ������ PINK ��
			m_nGame[2][6] == HUMAN_PINK    &&	m_nGame[4][6] == HUMAN_PINK &&
			m_nGame[6][6] == HUMAN_PINK    &&	m_nGame[8][6] == HUMAN_PINK &&
			m_nGame[3][7] == HUMAN_PINK    &&	m_nGame[5][7] == HUMAN_PINK &&
			m_nGame[7][7] == HUMAN_PINK    &&	m_nGame[4][8] == HUMAN_PINK &&
			m_nGame[6][8] == HUMAN_PINK    &&	m_nGame[5][9] == HUMAN_PINK &&
				// return HUMAN_RED;

			// CYAN ������ RED ��
			m_nGame[14][2] == HUMAN_RED    &&	m_nGame[13][3] == HUMAN_RED &&
			m_nGame[15][3] == HUMAN_RED    &&	m_nGame[12][4] == HUMAN_RED &&
			m_nGame[14][4] == HUMAN_RED    &&	m_nGame[16][4] == HUMAN_RED &&
			m_nGame[11][5] == HUMAN_RED    &&	m_nGame[13][5] == HUMAN_RED &&
			m_nGame[15][5] == HUMAN_RED    &&	m_nGame[17][5] == HUMAN_RED &&
				// return HUMAN_YELLOW;

			// BLUE ������ YELLOW ��
			m_nGame[20][6] == HUMAN_YELLOW &&	m_nGame[22][6] == HUMAN_YELLOW &&
			m_nGame[24][6] == HUMAN_YELLOW &&	m_nGame[26][6] == HUMAN_YELLOW &&
			m_nGame[21][7] == HUMAN_YELLOW &&	m_nGame[23][7] == HUMAN_YELLOW &&
			m_nGame[25][7] == HUMAN_YELLOW &&	m_nGame[22][8] == HUMAN_YELLOW &&
			m_nGame[24][8] == HUMAN_YELLOW &&	m_nGame[23][9] == HUMAN_YELLOW )
				return HUMAN_GREEN;

			else if( // RED ������ CYAN ��
			m_nGame[11][15] == HUMAN_CYAN  &&	m_nGame[13][15] == HUMAN_CYAN &&
			m_nGame[15][15] == HUMAN_CYAN  &&	m_nGame[17][15] == HUMAN_CYAN &&
			m_nGame[12][16] == HUMAN_CYAN  &&	m_nGame[14][16] == HUMAN_CYAN &&
			m_nGame[16][16] == HUMAN_CYAN  &&	m_nGame[13][17] == HUMAN_CYAN &&
			m_nGame[15][17] == HUMAN_CYAN  &&	m_nGame[14][18] == HUMAN_CYAN &&
				// return HUMAN_BLUE;

			// YELLOW ������ BLUE ��
			m_nGame[5][11] == HUMAN_BLUE   &&	m_nGame[4][12] == HUMAN_BLUE &&
			m_nGame[6][12] == HUMAN_BLUE   &&	m_nGame[3][13] == HUMAN_BLUE &&
			m_nGame[5][13] == HUMAN_BLUE   &&	m_nGame[7][13] == HUMAN_BLUE &&
			m_nGame[2][14] == HUMAN_BLUE   &&	m_nGame[4][14] == HUMAN_BLUE &&
			m_nGame[6][14] == HUMAN_BLUE   &&	m_nGame[8][14] == HUMAN_BLUE &&
				// return HUMAN_PINK;

			// GREEN ������ PINK ��
			m_nGame[2][6] == HUMAN_PINK    &&	m_nGame[4][6] == HUMAN_PINK &&
			m_nGame[6][6] == HUMAN_PINK    &&	m_nGame[8][6] == HUMAN_PINK &&
			m_nGame[3][7] == HUMAN_PINK    &&	m_nGame[5][7] == HUMAN_PINK &&
			m_nGame[7][7] == HUMAN_PINK    &&	m_nGame[4][8] == HUMAN_PINK &&
			m_nGame[6][8] == HUMAN_PINK    &&	m_nGame[5][9] == HUMAN_PINK &&
				// return HUMAN_RED;

			// CYAN ������ RED ��
			m_nGame[14][2] == HUMAN_RED    &&	m_nGame[13][3] == HUMAN_RED &&
			m_nGame[15][3] == HUMAN_RED    &&	m_nGame[12][4] == HUMAN_RED &&
			m_nGame[14][4] == HUMAN_RED    &&	m_nGame[16][4] == HUMAN_RED &&
			m_nGame[11][5] == HUMAN_RED    &&	m_nGame[13][5] == HUMAN_RED &&
			m_nGame[15][5] == HUMAN_RED    &&	m_nGame[17][5] == HUMAN_RED )
				return HUMAN_YELLOW;

			else if( // RED ������ CYAN ��
			m_nGame[11][15] == HUMAN_CYAN  &&	m_nGame[13][15] == HUMAN_CYAN &&
			m_nGame[15][15] == HUMAN_CYAN  &&	m_nGame[17][15] == HUMAN_CYAN &&
			m_nGame[12][16] == HUMAN_CYAN  &&	m_nGame[14][16] == HUMAN_CYAN &&
			m_nGame[16][16] == HUMAN_CYAN  &&	m_nGame[13][17] == HUMAN_CYAN &&
			m_nGame[15][17] == HUMAN_CYAN  &&	m_nGame[14][18] == HUMAN_CYAN &&
				// return HUMAN_BLUE;

			// YELLOW ������ BLUE ��
			m_nGame[5][11] == HUMAN_BLUE   &&	m_nGame[4][12] == HUMAN_BLUE &&
			m_nGame[6][12] == HUMAN_BLUE   &&	m_nGame[3][13] == HUMAN_BLUE &&
			m_nGame[5][13] == HUMAN_BLUE   &&	m_nGame[7][13] == HUMAN_BLUE &&
			m_nGame[2][14] == HUMAN_BLUE   &&	m_nGame[4][14] == HUMAN_BLUE &&
			m_nGame[6][14] == HUMAN_BLUE   &&	m_nGame[8][14] == HUMAN_BLUE &&
				// return HUMAN_PINK;

			// GREEN ������ PINK ��
			m_nGame[2][6] == HUMAN_PINK    &&	m_nGame[4][6] == HUMAN_PINK &&
			m_nGame[6][6] == HUMAN_PINK    &&	m_nGame[8][6] == HUMAN_PINK &&
			m_nGame[3][7] == HUMAN_PINK    &&	m_nGame[5][7] == HUMAN_PINK &&
			m_nGame[7][7] == HUMAN_PINK    &&	m_nGame[4][8] == HUMAN_PINK &&
			m_nGame[6][8] == HUMAN_PINK    &&	m_nGame[5][9] == HUMAN_PINK )
				return HUMAN_RED;

			else if( // RED ������ CYAN ��
			m_nGame[11][15] == HUMAN_CYAN  &&	m_nGame[13][15] == HUMAN_CYAN &&
			m_nGame[15][15] == HUMAN_CYAN  &&	m_nGame[17][15] == HUMAN_CYAN &&
			m_nGame[12][16] == HUMAN_CYAN  &&	m_nGame[14][16] == HUMAN_CYAN &&
			m_nGame[16][16] == HUMAN_CYAN  &&	m_nGame[13][17] == HUMAN_CYAN &&
			m_nGame[15][17] == HUMAN_CYAN  &&	m_nGame[14][18] == HUMAN_CYAN &&
				// return HUMAN_BLUE;

			// YELLOW ������ BLUE ��
			m_nGame[5][11] == HUMAN_BLUE   &&	m_nGame[4][12] == HUMAN_BLUE &&
			m_nGame[6][12] == HUMAN_BLUE   &&	m_nGame[3][13] == HUMAN_BLUE &&
			m_nGame[5][13] == HUMAN_BLUE   &&	m_nGame[7][13] == HUMAN_BLUE &&
			m_nGame[2][14] == HUMAN_BLUE   &&	m_nGame[4][14] == HUMAN_BLUE &&
			m_nGame[6][14] == HUMAN_BLUE   &&	m_nGame[8][14] == HUMAN_BLUE )
				return HUMAN_PINK;

			else if( // RED ������ CYAN ��
			m_nGame[11][15] == HUMAN_CYAN  &&	m_nGame[13][15] == HUMAN_CYAN &&
			m_nGame[15][15] == HUMAN_CYAN  &&	m_nGame[17][15] == HUMAN_CYAN &&
			m_nGame[12][16] == HUMAN_CYAN  &&	m_nGame[14][16] == HUMAN_CYAN &&
			m_nGame[16][16] == HUMAN_CYAN  &&	m_nGame[13][17] == HUMAN_CYAN &&
			m_nGame[15][17] == HUMAN_CYAN  &&	m_nGame[14][18] == HUMAN_CYAN )
				return HUMAN_BLUE;
		}

		else if(nType == HUMAN_BLUE) {
			if( // YELLOW ������ BLUE ��
			m_nGame[5][11] == HUMAN_BLUE   &&	m_nGame[4][12] == HUMAN_BLUE &&
			m_nGame[6][12] == HUMAN_BLUE   &&	m_nGame[3][13] == HUMAN_BLUE &&
			m_nGame[5][13] == HUMAN_BLUE   &&	m_nGame[7][13] == HUMAN_BLUE &&
			m_nGame[2][14] == HUMAN_BLUE   &&	m_nGame[4][14] == HUMAN_BLUE &&
			m_nGame[6][14] == HUMAN_BLUE   &&	m_nGame[8][14] == HUMAN_BLUE &&
				// return HUMAN_PINK;

			// GREEN ������ PINK ��
			m_nGame[2][6] == HUMAN_PINK    &&	m_nGame[4][6] == HUMAN_PINK &&
			m_nGame[6][6] == HUMAN_PINK    &&	m_nGame[8][6] == HUMAN_PINK &&
			m_nGame[3][7] == HUMAN_PINK    &&	m_nGame[5][7] == HUMAN_PINK &&
			m_nGame[7][7] == HUMAN_PINK    &&	m_nGame[4][8] == HUMAN_PINK &&
			m_nGame[6][8] == HUMAN_PINK    &&	m_nGame[5][9] == HUMAN_PINK &&
				// return HUMAN_RED;

			// CYAN ������ RED ��
			m_nGame[14][2] == HUMAN_RED    &&	m_nGame[13][3] == HUMAN_RED &&
			m_nGame[15][3] == HUMAN_RED    &&	m_nGame[12][4] == HUMAN_RED &&
			m_nGame[14][4] == HUMAN_RED    &&	m_nGame[16][4] == HUMAN_RED &&
			m_nGame[11][5] == HUMAN_RED    &&	m_nGame[13][5] == HUMAN_RED &&
			m_nGame[15][5] == HUMAN_RED    &&	m_nGame[17][5] == HUMAN_RED &&
				// return HUMAN_YELLOW;

			// BLUE ������ YELLOW ��
			m_nGame[20][6] == HUMAN_YELLOW &&	m_nGame[22][6] == HUMAN_YELLOW &&
			m_nGame[24][6] == HUMAN_YELLOW &&	m_nGame[26][6] == HUMAN_YELLOW &&
			m_nGame[21][7] == HUMAN_YELLOW &&	m_nGame[23][7] == HUMAN_YELLOW &&
			m_nGame[25][7] == HUMAN_YELLOW &&	m_nGame[22][8] == HUMAN_YELLOW &&
			m_nGame[24][8] == HUMAN_YELLOW &&	m_nGame[23][9] == HUMAN_YELLOW &&
				// return HUMAN_GREEN;

			// PINK ������ GREEN ��
			m_nGame[23][11] == HUMAN_GREEN &&	m_nGame[22][12] == HUMAN_GREEN &&
			m_nGame[24][12] == HUMAN_GREEN &&	m_nGame[21][13] == HUMAN_GREEN &&
			m_nGame[23][13] == HUMAN_GREEN &&	m_nGame[25][13] == HUMAN_GREEN &&
			m_nGame[20][14] == HUMAN_GREEN &&	m_nGame[22][14] == HUMAN_GREEN &&
			m_nGame[24][14] == HUMAN_GREEN &&	m_nGame[26][14] == HUMAN_GREEN )
				return HUMAN_CYAN;

			else if( // YELLOW ������ BLUE ��
			m_nGame[5][11] == HUMAN_BLUE   &&	m_nGame[4][12] == HUMAN_BLUE &&
			m_nGame[6][12] == HUMAN_BLUE   &&	m_nGame[3][13] == HUMAN_BLUE &&
			m_nGame[5][13] == HUMAN_BLUE   &&	m_nGame[7][13] == HUMAN_BLUE &&
			m_nGame[2][14] == HUMAN_BLUE   &&	m_nGame[4][14] == HUMAN_BLUE &&
			m_nGame[6][14] == HUMAN_BLUE   &&	m_nGame[8][14] == HUMAN_BLUE &&
				// return HUMAN_PINK;

			// GREEN ������ PINK ��
			m_nGame[2][6] == HUMAN_PINK    &&	m_nGame[4][6] == HUMAN_PINK &&
			m_nGame[6][6] == HUMAN_PINK    &&	m_nGame[8][6] == HUMAN_PINK &&
			m_nGame[3][7] == HUMAN_PINK    &&	m_nGame[5][7] == HUMAN_PINK &&
			m_nGame[7][7] == HUMAN_PINK    &&	m_nGame[4][8] == HUMAN_PINK &&
			m_nGame[6][8] == HUMAN_PINK    &&	m_nGame[5][9] == HUMAN_PINK &&
				// return HUMAN_RED;

			// CYAN ������ RED ��
			m_nGame[14][2] == HUMAN_RED    &&	m_nGame[13][3] == HUMAN_RED &&
			m_nGame[15][3] == HUMAN_RED    &&	m_nGame[12][4] == HUMAN_RED &&
			m_nGame[14][4] == HUMAN_RED    &&	m_nGame[16][4] == HUMAN_RED &&
			m_nGame[11][5] == HUMAN_RED    &&	m_nGame[13][5] == HUMAN_RED &&
			m_nGame[15][5] == HUMAN_RED    &&	m_nGame[17][5] == HUMAN_RED &&
				// return HUMAN_YELLOW;

			// BLUE ������ YELLOW ��
			m_nGame[20][6] == HUMAN_YELLOW &&	m_nGame[22][6] == HUMAN_YELLOW &&
			m_nGame[24][6] == HUMAN_YELLOW &&	m_nGame[26][6] == HUMAN_YELLOW &&
			m_nGame[21][7] == HUMAN_YELLOW &&	m_nGame[23][7] == HUMAN_YELLOW &&
			m_nGame[25][7] == HUMAN_YELLOW &&	m_nGame[22][8] == HUMAN_YELLOW &&
			m_nGame[24][8] == HUMAN_YELLOW &&	m_nGame[23][9] == HUMAN_YELLOW )
				return HUMAN_GREEN;

			else if( // YELLOW ������ BLUE ��
			m_nGame[5][11] == HUMAN_BLUE   &&	m_nGame[4][12] == HUMAN_BLUE &&
			m_nGame[6][12] == HUMAN_BLUE   &&	m_nGame[3][13] == HUMAN_BLUE &&
			m_nGame[5][13] == HUMAN_BLUE   &&	m_nGame[7][13] == HUMAN_BLUE &&
			m_nGame[2][14] == HUMAN_BLUE   &&	m_nGame[4][14] == HUMAN_BLUE &&
			m_nGame[6][14] == HUMAN_BLUE   &&	m_nGame[8][14] == HUMAN_BLUE &&
				// return HUMAN_PINK;

			// GREEN ������ PINK ��
			m_nGame[2][6] == HUMAN_PINK    &&	m_nGame[4][6] == HUMAN_PINK &&
			m_nGame[6][6] == HUMAN_PINK    &&	m_nGame[8][6] == HUMAN_PINK &&
			m_nGame[3][7] == HUMAN_PINK    &&	m_nGame[5][7] == HUMAN_PINK &&
			m_nGame[7][7] == HUMAN_PINK    &&	m_nGame[4][8] == HUMAN_PINK &&
			m_nGame[6][8] == HUMAN_PINK    &&	m_nGame[5][9] == HUMAN_PINK &&
				// return HUMAN_RED;

			// CYAN ������ RED ��
			m_nGame[14][2] == HUMAN_RED    &&	m_nGame[13][3] == HUMAN_RED &&
			m_nGame[15][3] == HUMAN_RED    &&	m_nGame[12][4] == HUMAN_RED &&
			m_nGame[14][4] == HUMAN_RED    &&	m_nGame[16][4] == HUMAN_RED &&
			m_nGame[11][5] == HUMAN_RED    &&	m_nGame[13][5] == HUMAN_RED &&
			m_nGame[15][5] == HUMAN_RED    &&	m_nGame[17][5] == HUMAN_RED )
				return HUMAN_YELLOW;

			else if( // YELLOW ������ BLUE ��
			m_nGame[5][11] == HUMAN_BLUE   &&	m_nGame[4][12] == HUMAN_BLUE &&
			m_nGame[6][12] == HUMAN_BLUE   &&	m_nGame[3][13] == HUMAN_BLUE &&
			m_nGame[5][13] == HUMAN_BLUE   &&	m_nGame[7][13] == HUMAN_BLUE &&
			m_nGame[2][14] == HUMAN_BLUE   &&	m_nGame[4][14] == HUMAN_BLUE &&
			m_nGame[6][14] == HUMAN_BLUE   &&	m_nGame[8][14] == HUMAN_BLUE &&
				// return HUMAN_PINK;

			// GREEN ������ PINK ��
			m_nGame[2][6] == HUMAN_PINK    &&	m_nGame[4][6] == HUMAN_PINK &&
			m_nGame[6][6] == HUMAN_PINK    &&	m_nGame[8][6] == HUMAN_PINK &&
			m_nGame[3][7] == HUMAN_PINK    &&	m_nGame[5][7] == HUMAN_PINK &&
			m_nGame[7][7] == HUMAN_PINK    &&	m_nGame[4][8] == HUMAN_PINK &&
			m_nGame[6][8] == HUMAN_PINK    &&	m_nGame[5][9] == HUMAN_PINK )
				return HUMAN_RED;

			else if( // YELLOW ������ BLUE ��
			m_nGame[5][11] == HUMAN_BLUE   &&	m_nGame[4][12] == HUMAN_BLUE &&
			m_nGame[6][12] == HUMAN_BLUE   &&	m_nGame[3][13] == HUMAN_BLUE &&
			m_nGame[5][13] == HUMAN_BLUE   &&	m_nGame[7][13] == HUMAN_BLUE &&
			m_nGame[2][14] == HUMAN_BLUE   &&	m_nGame[4][14] == HUMAN_BLUE &&
			m_nGame[6][14] == HUMAN_BLUE   &&	m_nGame[8][14] == HUMAN_BLUE )
				return HUMAN_PINK;
		}

		else if(nType == HUMAN_PINK) {
			if ( // GREEN ������ PINK ��
			m_nGame[2][6] == HUMAN_PINK    &&	m_nGame[4][6] == HUMAN_PINK &&
			m_nGame[6][6] == HUMAN_PINK    &&	m_nGame[8][6] == HUMAN_PINK &&
			m_nGame[3][7] == HUMAN_PINK    &&	m_nGame[5][7] == HUMAN_PINK &&
			m_nGame[7][7] == HUMAN_PINK    &&	m_nGame[4][8] == HUMAN_PINK &&
			m_nGame[6][8] == HUMAN_PINK    &&	m_nGame[5][9] == HUMAN_PINK &&
				// return HUMAN_RED;

			// CYAN ������ RED ��
			m_nGame[14][2] == HUMAN_RED    &&	m_nGame[13][3] == HUMAN_RED &&
			m_nGame[15][3] == HUMAN_RED    &&	m_nGame[12][4] == HUMAN_RED &&
			m_nGame[14][4] == HUMAN_RED    &&	m_nGame[16][4] == HUMAN_RED &&
			m_nGame[11][5] == HUMAN_RED    &&	m_nGame[13][5] == HUMAN_RED &&
			m_nGame[15][5] == HUMAN_RED    &&	m_nGame[17][5] == HUMAN_RED &&
				// return HUMAN_YELLOW;

			// BLUE ������ YELLOW ��
			m_nGame[20][6] == HUMAN_YELLOW &&	m_nGame[22][6] == HUMAN_YELLOW &&
			m_nGame[24][6] == HUMAN_YELLOW &&	m_nGame[26][6] == HUMAN_YELLOW &&
			m_nGame[21][7] == HUMAN_YELLOW &&	m_nGame[23][7] == HUMAN_YELLOW &&
			m_nGame[25][7] == HUMAN_YELLOW &&	m_nGame[22][8] == HUMAN_YELLOW &&
			m_nGame[24][8] == HUMAN_YELLOW &&	m_nGame[23][9] == HUMAN_YELLOW &&
				// return HUMAN_GREEN;

			// PINK ������ GREEN ��
			m_nGame[23][11] == HUMAN_GREEN &&	m_nGame[22][12] == HUMAN_GREEN &&
			m_nGame[24][12] == HUMAN_GREEN &&	m_nGame[21][13] == HUMAN_GREEN &&
			m_nGame[23][13] == HUMAN_GREEN &&	m_nGame[25][13] == HUMAN_GREEN &&
			m_nGame[20][14] == HUMAN_GREEN &&	m_nGame[22][14] == HUMAN_GREEN &&
			m_nGame[24][14] == HUMAN_GREEN &&	m_nGame[26][14] == HUMAN_GREEN &&
				// return HUMAN_CYAN;

			// RED ������ CYAN ��
			m_nGame[11][15] == HUMAN_CYAN  &&	m_nGame[13][15] == HUMAN_CYAN &&
			m_nGame[15][15] == HUMAN_CYAN  &&	m_nGame[17][15] == HUMAN_CYAN &&
			m_nGame[12][16] == HUMAN_CYAN  &&	m_nGame[14][16] == HUMAN_CYAN &&
			m_nGame[16][16] == HUMAN_CYAN  &&	m_nGame[13][17] == HUMAN_CYAN &&
			m_nGame[15][17] == HUMAN_CYAN  &&	m_nGame[14][18] == HUMAN_CYAN )
				return HUMAN_BLUE;

			else if( // GREEN ������ PINK ��
			m_nGame[2][6] == HUMAN_PINK    &&	m_nGame[4][6] == HUMAN_PINK &&
			m_nGame[6][6] == HUMAN_PINK    &&	m_nGame[8][6] == HUMAN_PINK &&
			m_nGame[3][7] == HUMAN_PINK    &&	m_nGame[5][7] == HUMAN_PINK &&
			m_nGame[7][7] == HUMAN_PINK    &&	m_nGame[4][8] == HUMAN_PINK &&
			m_nGame[6][8] == HUMAN_PINK    &&	m_nGame[5][9] == HUMAN_PINK &&
				// return HUMAN_RED;

			// CYAN ������ RED ��
			m_nGame[14][2] == HUMAN_RED    &&	m_nGame[13][3] == HUMAN_RED &&
			m_nGame[15][3] == HUMAN_RED    &&	m_nGame[12][4] == HUMAN_RED &&
			m_nGame[14][4] == HUMAN_RED    &&	m_nGame[16][4] == HUMAN_RED &&
			m_nGame[11][5] == HUMAN_RED    &&	m_nGame[13][5] == HUMAN_RED &&
			m_nGame[15][5] == HUMAN_RED    &&	m_nGame[17][5] == HUMAN_RED &&
				// return HUMAN_YELLOW;

			// BLUE ������ YELLOW ��
			m_nGame[20][6] == HUMAN_YELLOW &&	m_nGame[22][6] == HUMAN_YELLOW &&
			m_nGame[24][6] == HUMAN_YELLOW &&	m_nGame[26][6] == HUMAN_YELLOW &&
			m_nGame[21][7] == HUMAN_YELLOW &&	m_nGame[23][7] == HUMAN_YELLOW &&
			m_nGame[25][7] == HUMAN_YELLOW &&	m_nGame[22][8] == HUMAN_YELLOW &&
			m_nGame[24][8] == HUMAN_YELLOW &&	m_nGame[23][9] == HUMAN_YELLOW &&
				// return HUMAN_GREEN;

			// PINK ������ GREEN ��
			m_nGame[23][11] == HUMAN_GREEN &&	m_nGame[22][12] == HUMAN_GREEN &&
			m_nGame[24][12] == HUMAN_GREEN &&	m_nGame[21][13] == HUMAN_GREEN &&
			m_nGame[23][13] == HUMAN_GREEN &&	m_nGame[25][13] == HUMAN_GREEN &&
			m_nGame[20][14] == HUMAN_GREEN &&	m_nGame[22][14] == HUMAN_GREEN &&
			m_nGame[24][14] == HUMAN_GREEN &&	m_nGame[26][14] == HUMAN_GREEN )
				return HUMAN_CYAN;

			else if( // GREEN ������ PINK ��
			m_nGame[2][6] == HUMAN_PINK    &&	m_nGame[4][6] == HUMAN_PINK &&
			m_nGame[6][6] == HUMAN_PINK    &&	m_nGame[8][6] == HUMAN_PINK &&
			m_nGame[3][7] == HUMAN_PINK    &&	m_nGame[5][7] == HUMAN_PINK &&
			m_nGame[7][7] == HUMAN_PINK    &&	m_nGame[4][8] == HUMAN_PINK &&
			m_nGame[6][8] == HUMAN_PINK    &&	m_nGame[5][9] == HUMAN_PINK &&
				// return HUMAN_RED;

			// CYAN ������ RED ��
			m_nGame[14][2] == HUMAN_RED    &&	m_nGame[13][3] == HUMAN_RED &&
			m_nGame[15][3] == HUMAN_RED    &&	m_nGame[12][4] == HUMAN_RED &&
			m_nGame[14][4] == HUMAN_RED    &&	m_nGame[16][4] == HUMAN_RED &&
			m_nGame[11][5] == HUMAN_RED    &&	m_nGame[13][5] == HUMAN_RED &&
			m_nGame[15][5] == HUMAN_RED    &&	m_nGame[17][5] == HUMAN_RED &&
				// return HUMAN_YELLOW;

			// BLUE ������ YELLOW ��
			m_nGame[20][6] == HUMAN_YELLOW &&	m_nGame[22][6] == HUMAN_YELLOW &&
			m_nGame[24][6] == HUMAN_YELLOW &&	m_nGame[26][6] == HUMAN_YELLOW &&
			m_nGame[21][7] == HUMAN_YELLOW &&	m_nGame[23][7] == HUMAN_YELLOW &&
			m_nGame[25][7] == HUMAN_YELLOW &&	m_nGame[22][8] == HUMAN_YELLOW &&
			m_nGame[24][8] == HUMAN_YELLOW &&	m_nGame[23][9] == HUMAN_YELLOW )
				return HUMAN_GREEN;

			else if( // GREEN ������ PINK ��
			m_nGame[2][6] == HUMAN_PINK    &&	m_nGame[4][6] == HUMAN_PINK &&
			m_nGame[6][6] == HUMAN_PINK    &&	m_nGame[8][6] == HUMAN_PINK &&
			m_nGame[3][7] == HUMAN_PINK    &&	m_nGame[5][7] == HUMAN_PINK &&
			m_nGame[7][7] == HUMAN_PINK    &&	m_nGame[4][8] == HUMAN_PINK &&
			m_nGame[6][8] == HUMAN_PINK    &&	m_nGame[5][9] == HUMAN_PINK &&
				// return HUMAN_RED;

			// CYAN ������ RED ��
			m_nGame[14][2] == HUMAN_RED    &&	m_nGame[13][3] == HUMAN_RED &&
			m_nGame[15][3] == HUMAN_RED    &&	m_nGame[12][4] == HUMAN_RED &&
			m_nGame[14][4] == HUMAN_RED    &&	m_nGame[16][4] == HUMAN_RED &&
			m_nGame[11][5] == HUMAN_RED    &&	m_nGame[13][5] == HUMAN_RED &&
			m_nGame[15][5] == HUMAN_RED    &&	m_nGame[17][5] == HUMAN_RED )
				return HUMAN_YELLOW;

			else if( // GREEN ������ PINK ��
			m_nGame[2][6] == HUMAN_PINK    &&	m_nGame[4][6] == HUMAN_PINK &&
			m_nGame[6][6] == HUMAN_PINK    &&	m_nGame[8][6] == HUMAN_PINK &&
			m_nGame[3][7] == HUMAN_PINK    &&	m_nGame[5][7] == HUMAN_PINK &&
			m_nGame[7][7] == HUMAN_PINK    &&	m_nGame[4][8] == HUMAN_PINK &&
			m_nGame[6][8] == HUMAN_PINK    &&	m_nGame[5][9] == HUMAN_PINK )
				return HUMAN_RED;
		}

		return nType;
		break;

	default :
		AfxMessageBox("���� �ο����� ���� �ʽ��ϴ�.");
		break;
	}
	return 0;
}

void CPrinceGameDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	int nCurX, nCurY;
	int i, j;

	if(m_nStats == 0) {
		// ���� �����Ǿ� �ִ� ���¿��� LButtonDown�� ����

		for(i=0; i<29; i++) {
			for(j=0; j<21; j++) {
				if(point.x >= m_rectArray[i][j].left && point.x <= m_rectArray[i][j].right && 
				point.y >= m_rectArray[i][j].top && point.y <= m_rectArray[i][j].bottom) {
				
					// x�� y�� ���
					nCurX = i;
					nCurY = j;

					if(m_nGame[nCurX][nCurY] == m_nCurType) {

						// x�� y�� ��������� ���� ���Ͽ� �������� ��ǥ ���
						m_nOldX = nCurX;
						m_nOldY = nCurY;

						// �´��� Ȯ���ϱ� ���Ͽ� �޽����ڽ� ȣ��
						// CString str;
						// str.Format("%d, %d", nCurX, nCurY);
						// AfxMessageBox(str);

						/* -----------------------------------------
						���� ���� ��ġ���� �ڱ� ������ ���� ���¸� 
						���� ���� ���ٸ� ACTIVATE ������ �־��� 
						----------------------------------------- */
						// array[i+2][j]    ��		 1ĭ�̵�
						// array[i-2][j]    ��		 1ĭ�̵�
						// array[i-1][j-1]  ��		 1ĭ�̵�
						// array[i-1][j+1]  ��       1ĭ�̵�
						// array[i+1][j+1]  ��       1ĭ�̵�
						// array[i+1][j-1]  ��       1ĭ�̵�

						if(m_nGame[nCurX+2][nCurY] == EMPTY) {
							m_nGame[nCurX+2][nCurY] = ACTIVATE1;
						} 
						if(m_nGame[nCurX-2][nCurY] == EMPTY) {
							m_nGame[nCurX-2][nCurY] = ACTIVATE1;
						} 
						if(m_nGame[nCurX-1][nCurY-1] == EMPTY) {
							m_nGame[nCurX-1][nCurY-1] = ACTIVATE1;
						} 
						if(m_nGame[nCurX-1][nCurY+1] == EMPTY) {
							m_nGame[nCurX-1][nCurY+1] = ACTIVATE1;
						} 
						if(m_nGame[nCurX+1][nCurY+1] == EMPTY) {
							m_nGame[nCurX+1][nCurY+1] = ACTIVATE1;
						} 
						if(m_nGame[nCurX+1][nCurY-1] == EMPTY) {
							m_nGame[nCurX+1][nCurY-1] = ACTIVATE1;
						}

						// array[i+4][j]    ��	     2ĭ�̵�
						// array[i-4][j]    ��	     2ĭ�̵�
						// array[i-2][j-2]  ��       2ĭ�̵�
						// array[i-2][j+2]  ��       2ĭ�̵�
						// array[i+2][j+2]  ��       2ĭ�̵�
						// array[i+2][j-2]  ��       2ĭ�̵�

						if((m_nGame[nCurX+2][nCurY] != EMPTY && m_nGame[nCurX+2][nCurY] != ACTIVATE1 && m_nGame[nCurX+2][nCurY] != ACTIVATE2) && m_nGame[nCurX+4][nCurY] == EMPTY) {
							m_nGame[nCurX+4][nCurY] = ACTIVATE2;
						} 
						if((m_nGame[nCurX-2][nCurY] != EMPTY && m_nGame[nCurX-2][nCurY] != ACTIVATE1 && m_nGame[nCurX-2][nCurY] != ACTIVATE2) && m_nGame[nCurX-4][nCurY] == EMPTY) {
							m_nGame[nCurX-4][nCurY] = ACTIVATE2;
						} 
						if((m_nGame[nCurX-1][nCurY-1] != EMPTY && m_nGame[nCurX-1][nCurY-1] != ACTIVATE1 && m_nGame[nCurX-1][nCurY-1] != ACTIVATE2) && m_nGame[nCurX-2][nCurY-2] == EMPTY) {
							m_nGame[nCurX-2][nCurY-2] = ACTIVATE2;
						} 
						if((m_nGame[nCurX-1][nCurY+1] != EMPTY && m_nGame[nCurX-1][nCurY+1] != ACTIVATE1 && m_nGame[nCurX-1][nCurY+1] != ACTIVATE2) && m_nGame[nCurX-2][nCurY+2] == EMPTY) {
							m_nGame[nCurX-2][nCurY+2] = ACTIVATE2;
						} 
						if((m_nGame[nCurX+1][nCurY+1] != EMPTY && m_nGame[nCurX+1][nCurY+1] != ACTIVATE1 && m_nGame[nCurX+1][nCurY+1] != ACTIVATE2) && m_nGame[nCurX+2][nCurY+2] == EMPTY) {
							m_nGame[nCurX+2][nCurY+2] = ACTIVATE2;
						} 
						if((m_nGame[nCurX+1][nCurY-1] != EMPTY && m_nGame[nCurX+1][nCurY-1] != ACTIVATE1 && m_nGame[nCurX+1][nCurY-1] != ACTIVATE2) && m_nGame[nCurX+2][nCurY-2] == EMPTY) {
							m_nGame[nCurX+2][nCurY-2] = ACTIVATE2;
						}

						// ������ �׷��� ��Ʈ�� �����
						if(m_nGame[nCurX][nCurY] != EMPTY && (m_nGame[nCurX][nCurY] != ACTIVATE1 || m_nGame[nCurX][nCurY] != ACTIVATE2)) {
							m_nGame[nCurX][nCurY] = EMPTY;
							m_nStats = 1;
							m_bCheckMove = TRUE;
						}
					}
				}
			}
		}

	} else {
		// ���� �����̰� �ִ� ���¿��� LButtonDown�� ����

		for(i=0; i<29; i++) {
			for(j=0; j<21; j++) {
				if(point.x >= m_rectArray[i][j].left && point.x <= m_rectArray[i][j].right && 
				point.y >= m_rectArray[i][j].top && point.y <= m_rectArray[i][j].bottom) {
						
					// x�� y�� ���
					nCurX = i;
					nCurY = j;

					// �´��� Ȯ���ϱ� ���Ͽ� �޽����ڽ� ȣ��
					// CString str;
					// str.Format("%d, %d", m_nGame[i][j], nCurY);
					// AfxMessageBox(str);

					// ������ ACTIVATE �����̸� ���� 
					if(m_nGame[nCurX][nCurY] == ACTIVATE2) {

						// ó�� Ŭ���������� ���� ���¸� ACTIVATE2�� �ʱ�ȭ
						m_nGame[m_nOldX][m_nOldY] = ACTIVATE2;

						// ACTIVATE1�� ���ִ� ���ǵ� EMPTY�� �ʱ�ȭ
						for(i=0; i<29; i++) {
						 	for(j=0; j<21; j++) {
								if(m_nGame[i][j] == ACTIVATE1)
									m_nGame[i][j] = EMPTY;
							}
						}

						if(((m_nGame[nCurX+2][nCurY] != EMPTY && m_nGame[nCurX+2][nCurY] != ACTIVATE1 && m_nGame[nCurX+2][nCurY] != ACTIVATE2) && m_nGame[nCurX+4][nCurY] == EMPTY) ||
						((m_nGame[nCurX-2][nCurY] != EMPTY && m_nGame[nCurX-2][nCurY] != ACTIVATE1 && m_nGame[nCurX-2][nCurY] != ACTIVATE2) && m_nGame[nCurX-4][nCurY] == EMPTY) ||
						((m_nGame[nCurX-1][nCurY-1] != EMPTY && m_nGame[nCurX-1][nCurY-1] != ACTIVATE1 && m_nGame[nCurX-1][nCurY-1] != ACTIVATE2) && m_nGame[nCurX-2][nCurY-2] == EMPTY) ||
						((m_nGame[nCurX-1][nCurY+1] != EMPTY && m_nGame[nCurX-1][nCurY+1] != ACTIVATE1 && m_nGame[nCurX-1][nCurY+1] != ACTIVATE2) && m_nGame[nCurX-2][nCurY+2] == EMPTY) ||
						((m_nGame[nCurX+1][nCurY+1] != EMPTY && m_nGame[nCurX+1][nCurY+1] != ACTIVATE1 && m_nGame[nCurX+1][nCurY+1] != ACTIVATE2) && m_nGame[nCurX+2][nCurY+2] == EMPTY) ||
						((m_nGame[nCurX+1][nCurY-1] != EMPTY && m_nGame[nCurX+1][nCurY-1] != ACTIVATE1 && m_nGame[nCurX+1][nCurY-1] != ACTIVATE2) && m_nGame[nCurX+2][nCurY-2] == EMPTY)
						) {
								
							// �� �κ��� ��Ÿ�� ������ ACTIVTE ����
							if((m_nGame[nCurX+2][nCurY] != EMPTY && m_nGame[nCurX+2][nCurY] != ACTIVATE1 && m_nGame[nCurX+2][nCurY] != ACTIVATE2) && m_nGame[nCurX+4][nCurY] == EMPTY) {
								m_nGame[nCurX+4][nCurY] = ACTIVATE2;
							} 
							if((m_nGame[nCurX-2][nCurY] != EMPTY && m_nGame[nCurX-2][nCurY] != ACTIVATE1 && m_nGame[nCurX-2][nCurY] != ACTIVATE2) && m_nGame[nCurX-4][nCurY] == EMPTY) {
								m_nGame[nCurX-4][nCurY] = ACTIVATE2;
							} 
							if((m_nGame[nCurX-1][nCurY-1] != EMPTY && m_nGame[nCurX-1][nCurY-1] != ACTIVATE1 && m_nGame[nCurX-1][nCurY-1] != ACTIVATE2) && m_nGame[nCurX-2][nCurY-2] == EMPTY) {
								m_nGame[nCurX-2][nCurY-2] = ACTIVATE2;
							} 
							if((m_nGame[nCurX-1][nCurY+1] != EMPTY && m_nGame[nCurX-1][nCurY+1] != ACTIVATE1 && m_nGame[nCurX-1][nCurY+1] != ACTIVATE2) && m_nGame[nCurX-2][nCurY+2] == EMPTY) {
								m_nGame[nCurX-2][nCurY+2] = ACTIVATE2;
							} 
							if((m_nGame[nCurX+1][nCurY+1] != EMPTY && m_nGame[nCurX+1][nCurY+1] != ACTIVATE1 && m_nGame[nCurX+1][nCurY+1] != ACTIVATE2) && m_nGame[nCurX+2][nCurY+2] == EMPTY) {
								m_nGame[nCurX+2][nCurY+2] = ACTIVATE2;
							} 
							if((m_nGame[nCurX+1][nCurY-1] != EMPTY && m_nGame[nCurX+1][nCurY-1] != ACTIVATE1 && m_nGame[nCurX+1][nCurY-1] != ACTIVATE2) && m_nGame[nCurX+2][nCurY-2] == EMPTY) {
								m_nGame[nCurX+2][nCurY-2] = ACTIVATE2;
							}

							// Sound Move
							PlaySound(IDR_MOVE);
							
							// ���� ��Ÿ �ǰ��Ϸ��� �Ʒ��ҽ��� �ִ´�
							// m_nGame[nCurX][nCurY] = EMPTY;

							// �´��� Ȯ���ϱ� ���Ͽ� �޽����ڽ� ȣ��
							// AfxMessageBox("Ȯ��");

						} else {
							
							// �ǳʶ� �� ������ ������
							// CClientDC dc(this);
							// CDC memDC;
							// memDC.CreateCompatibleDC(&dc);

							// CBitmap memBMP, *pOldBitmap;
							// memBMP.CreateCompatibleBitmap(&dc, 23, 23);

							// pOldBitmap = (CBitmap *)memDC.SelectObject(&m_bmpDlg);
							// dc.BitBlt(m_pt1.x-(23/2), m_pt1.y-(23/2), 23, 23, &memDC, m_pt1.x-(23/2), m_pt1.y-(23/2), SRCCOPY);
							// memDC.SelectObject(pOldBitmap);

							m_nGame[nCurX][nCurY] = m_nCurType;
							m_nStats = 0;
							m_bCheckMove = FALSE;

							// ACTIVATE�� ���ִ� ���ǵ� EMPTY�� �ʱ�ȭ
							for(i=0; i<29; i++) {
							 	for(j=0; j<21; j++) {
									if(m_nGame[i][j] == ACTIVATE1 || m_nGame[i][j] == ACTIVATE2)
										m_nGame[i][j] = EMPTY;
								}
							}

							// ���� ���� ��ġ
							m_nCurX = nCurX;
							m_nCurY = nCurY;

							// Sound Move
							PlaySound(IDR_MOVE);

							// ������ üũ
							if(m_bCheckItem) {
								if(m_nRand == 1) {
									if(m_bCurItem[0]) {
										if(nCurX==18 && nCurY==8) {
											//////////////////////////////////////////////////////////////////////
											m_nCurType = GetOtherType(m_nCurType);
											m_bEventItem[0] = TRUE;	// �ٳ��� ������

											PlaySound(IDR_ITEM);
											//////////////////////////////////////////////////////////////////////
											m_bCurItem[0] = FALSE;
										}
									}
									if(m_bCurItem[1]) {
										if(nCurX==11 && nCurY==9) {
											//////////////////////////////////////////////////////////////////////
											InitGame();
											m_bEventItem[2] = TRUE;	// ���� ������

											PlaySound(IDR_ITEM);
											//////////////////////////////////////////////////////////////////////
											m_bCurItem[1] = FALSE;
										}
									}
									if(m_bCurItem[2]) {
										if(nCurX==14 && nCurY==10) {
											//////////////////////////////////////////////////////////////////////
											m_nCurType = GetOtherType(m_nCurType);
											m_nCurType = GetOtherType(m_nCurType);
											m_nCurType = GetOtherType(m_nCurType);
											m_nCurType = GetOtherType(m_nCurType);
											m_nCurType = GetOtherType(m_nCurType);
											m_bEventItem[1] = TRUE;	// ���� ������

											PlaySound(IDR_ITEM);
											//////////////////////////////////////////////////////////////////////
											m_bCurItem[2] = FALSE;
										}
									}
									if(m_bCurItem[3]) {
										if(nCurX==12 && nCurY==12) {
											//////////////////////////////////////////////////////////////////////
											m_nCurType = GetOtherType(m_nCurType);
											m_nCurType = GetOtherType(m_nCurType);
											m_nCurType = GetOtherType(m_nCurType);
											m_nCurType = GetOtherType(m_nCurType);
											m_nCurType = GetOtherType(m_nCurType);
											m_bEventItem[1] = TRUE;	// ���� ������

											PlaySound(IDR_ITEM);
											//////////////////////////////////////////////////////////////////////
											m_bCurItem[3] = FALSE;
										}
									}
									if(m_bCurItem[4]) {
										if(nCurX==18 && nCurY==12) {
											//////////////////////////////////////////////////////////////////////
											m_nGame[nCurX][nCurY] = EMPTY;
											m_nGame[m_nOldX][m_nOldY] = m_nCurType;
											m_nStats = 0;
											m_bCheckMove = FALSE;

											// ACTIVATE�� ���ִ� ���ǵ� EMPTY�� �ʱ�ȭ
											for(int i=0; i<29; i++) {
			 									for(int j=0; j<21; j++) {
													if(m_nGame[i][j] == ACTIVATE1 || m_nGame[i][j] == ACTIVATE2)
														m_nGame[i][j] = EMPTY;
												}
											}
											m_bEventItem[3] = TRUE;	// ��ź ������

											PlaySound(IDR_ITEM);
											//////////////////////////////////////////////////////////////////////
											m_bCurItem[4] = FALSE;
										}
									}
								}
								else if(m_nRand == 2) {
									if(m_bCurItem[0]) {
										if(nCurX==11 && nCurY==7) {
											//////////////////////////////////////////////////////////////////////
											m_nCurType = GetOtherType(m_nCurType);
											m_nCurType = GetOtherType(m_nCurType);
											m_nCurType = GetOtherType(m_nCurType);
											m_nCurType = GetOtherType(m_nCurType);
											m_nCurType = GetOtherType(m_nCurType);
											m_bEventItem[1] = TRUE;	// ���� ������

											PlaySound(IDR_ITEM);
											//////////////////////////////////////////////////////////////////////
											m_bCurItem[0] = FALSE;
										}
									}
									if(m_bCurItem[1]) {
										if(nCurX==15 && nCurY==9) {
											//////////////////////////////////////////////////////////////////////
											m_nCurType = GetOtherType(m_nCurType);
											m_nCurType = GetOtherType(m_nCurType);
											m_nCurType = GetOtherType(m_nCurType);
											m_nCurType = GetOtherType(m_nCurType);
											m_nCurType = GetOtherType(m_nCurType);
											m_bEventItem[1] = TRUE;	// ���� ������

											PlaySound(IDR_ITEM);
											//////////////////////////////////////////////////////////////////////
											m_bCurItem[1] = FALSE;
										}
									}
									if(m_bCurItem[2]) {
										if(nCurX==12 && nCurY==10) {
											//////////////////////////////////////////////////////////////////////
											m_nGame[nCurX][nCurY] = EMPTY;
											m_nGame[m_nOldX][m_nOldY] = m_nCurType;
											m_nStats = 0;
											m_bCheckMove = FALSE;

											// ACTIVATE�� ���ִ� ���ǵ� EMPTY�� �ʱ�ȭ
											for(int i=0; i<29; i++) {
			 									for(int j=0; j<21; j++) {
													if(m_nGame[i][j] == ACTIVATE1 || m_nGame[i][j] == ACTIVATE2)
														m_nGame[i][j] = EMPTY;
												}
											}
											m_bEventItem[3] = TRUE;	// ��ź ������

											PlaySound(IDR_ITEM);
											//////////////////////////////////////////////////////////////////////
											m_bCurItem[2] = FALSE;
										}
									}
									if(m_bCurItem[3]) {
										if(nCurX==9 && nCurY==11) {
											//////////////////////////////////////////////////////////////////////
											m_nCurType = GetOtherType(m_nCurType);
											m_bEventItem[0] = TRUE;	// �ٳ��� ������

											PlaySound(IDR_ITEM);
											//////////////////////////////////////////////////////////////////////
											m_bCurItem[3] = FALSE;
										}
									}
									if(m_bCurItem[4]) {
										if(nCurX==17 && nCurY==11) {
											//////////////////////////////////////////////////////////////////////
											m_nCurType = GetOtherType(m_nCurType);
											m_bEventItem[0] = TRUE;	// �ٳ��� ������

											PlaySound(IDR_ITEM);
											//////////////////////////////////////////////////////////////////////
											m_bCurItem[4] = FALSE;
										}
									}
								}
								else if(m_nRand == 3) {
									if(m_bCurItem[0]) {
										if(nCurX==14 && nCurY==8) {
											//////////////////////////////////////////////////////////////////////
											m_nCurType = GetOtherType(m_nCurType);
											m_bEventItem[0] = TRUE;	// �ٳ��� ������

											PlaySound(IDR_ITEM);
											//////////////////////////////////////////////////////////////////////
											m_bCurItem[0] = FALSE;
										}
									}
									if(m_bCurItem[1]) {
										if(nCurX==12 && nCurY==10) {
											//////////////////////////////////////////////////////////////////////
											m_nGame[nCurX][nCurY] = EMPTY;
											m_nGame[m_nOldX][m_nOldY] = m_nCurType;
											m_nStats = 0;
											m_bCheckMove = FALSE;

											// ACTIVATE�� ���ִ� ���ǵ� EMPTY�� �ʱ�ȭ
											for(int i=0; i<29; i++) {
			 									for(int j=0; j<21; j++) {
													if(m_nGame[i][j] == ACTIVATE1 || m_nGame[i][j] == ACTIVATE2)
														m_nGame[i][j] = EMPTY;
												}
											}
											m_bEventItem[3] = TRUE;	// ��ź ������

											PlaySound(IDR_ITEM);
											//////////////////////////////////////////////////////////////////////
											m_bCurItem[1] = FALSE;
										}
									}
									if(m_bCurItem[2]) {
										if(nCurX==14 && nCurY==10) {
											//////////////////////////////////////////////////////////////////////
											m_nCurType = GetOtherType(m_nCurType);
											m_bEventItem[0] = TRUE;	// �ٳ��� ������

											PlaySound(IDR_ITEM);
											//////////////////////////////////////////////////////////////////////
											m_bCurItem[2] = FALSE;
										}
									}
									if(m_bCurItem[3]) {
										if(nCurX==16 && nCurY==10) {
											//////////////////////////////////////////////////////////////////////
											m_nCurType = GetOtherType(m_nCurType);
											m_nCurType = GetOtherType(m_nCurType);
											m_nCurType = GetOtherType(m_nCurType);
											m_nCurType = GetOtherType(m_nCurType);
											m_nCurType = GetOtherType(m_nCurType);
											m_bEventItem[1] = TRUE;	// ���� ������

											PlaySound(IDR_ITEM);
											//////////////////////////////////////////////////////////////////////
											m_bCurItem[3] = FALSE;
										}
									}
									if(m_bCurItem[4]) {
										if(nCurX==14 && nCurY==12) {
											//////////////////////////////////////////////////////////////////////
											m_nCurType = GetOtherType(m_nCurType);
											m_bEventItem[0] = TRUE;	// �ٳ��� ������

											PlaySound(IDR_ITEM);
											//////////////////////////////////////////////////////////////////////
											m_bCurItem[4] = FALSE;
										}
									}
								}
							}

							// GameOver Check
							if(CheckGameOver()) {
								GameOver();
							}

							// Ÿ���� ���ϹǷ�
							m_bCheckType = TRUE;

							// ���������� �ѱ�
							m_nCurType = GetOtherType(m_nCurType);
						}

					} else if (m_nGame[nCurX][nCurY] == ACTIVATE1) {

						// ���� ��ĭ�� ������ ��
						// CClientDC dc(this);
						// CDC memDC;
						// memDC.CreateCompatibleDC(&dc);

						// CBitmap memBMP, *pOldBitmap;
						// memBMP.CreateCompatibleBitmap(&dc, 23, 23);

						// pOldBitmap = (CBitmap *)memDC.SelectObject(&m_bmpDlg);
						// dc.BitBlt(m_pt1.x-(23/2), m_pt1.y-(23/2), 23, 23, &memDC, m_pt1.x-(23/2), m_pt1.y-(23/2), SRCCOPY);

						// memDC.SelectObject(pOldBitmap);

						m_nGame[nCurX][nCurY] = m_nCurType;
						m_nStats = 0;
						m_bCheckMove = FALSE;

						// ACTIVATE�� ���ִ� ���ǵ� EMPTY�� �ʱ�ȭ
						for(i=0; i<29; i++) {
						 	for(j=0; j<21; j++) {
								if(m_nGame[i][j] == ACTIVATE1 || m_nGame[i][j] == ACTIVATE2)
									m_nGame[i][j] = EMPTY;
							}
						}

						// ���� ���� ��ġ
						m_nCurX = nCurX;
						m_nCurY = nCurY;

						// Sound Move
						PlaySound(IDR_MOVE);

						// ������ üũ
							if(m_bCheckItem) {
								if(m_nRand == 1) {
									if(m_bCurItem[0]) {
										if(nCurX==18 && nCurY==8) {
											//////////////////////////////////////////////////////////////////////
											m_nCurType = GetOtherType(m_nCurType);
											m_bEventItem[0] = TRUE;	// �ٳ��� ������

											PlaySound(IDR_ITEM);
											//////////////////////////////////////////////////////////////////////
											m_bCurItem[0] = FALSE;
										}
									}
									if(m_bCurItem[1]) {
										if(nCurX==11 && nCurY==9) {
											//////////////////////////////////////////////////////////////////////
											InitGame();
											m_bEventItem[2] = TRUE;	// ���� ������

											PlaySound(IDR_ITEM);
											//////////////////////////////////////////////////////////////////////
											m_bCurItem[1] = FALSE;
										}
									}
									if(m_bCurItem[2]) {
										if(nCurX==14 && nCurY==10) {
											//////////////////////////////////////////////////////////////////////
											m_nCurType = GetOtherType(m_nCurType);
											m_nCurType = GetOtherType(m_nCurType);
											m_nCurType = GetOtherType(m_nCurType);
											m_nCurType = GetOtherType(m_nCurType);
											m_nCurType = GetOtherType(m_nCurType);
											m_bEventItem[1] = TRUE;	// ���� ������

											PlaySound(IDR_ITEM);
											//////////////////////////////////////////////////////////////////////
											m_bCurItem[2] = FALSE;
										}
									}
									if(m_bCurItem[3]) {
										if(nCurX==12 && nCurY==12) {
											//////////////////////////////////////////////////////////////////////
											m_nCurType = GetOtherType(m_nCurType);
											m_nCurType = GetOtherType(m_nCurType);
											m_nCurType = GetOtherType(m_nCurType);
											m_nCurType = GetOtherType(m_nCurType);
											m_nCurType = GetOtherType(m_nCurType);
											m_bEventItem[1] = TRUE;	// ���� ������

											PlaySound(IDR_ITEM);
											//////////////////////////////////////////////////////////////////////
											m_bCurItem[3] = FALSE;
										}
									}
									if(m_bCurItem[4]) {
										if(nCurX==18 && nCurY==12) {
											//////////////////////////////////////////////////////////////////////
											m_nGame[nCurX][nCurY] = EMPTY;
											m_nGame[m_nOldX][m_nOldY] = m_nCurType;
											m_nStats = 0;
											m_bCheckMove = FALSE;

											// ACTIVATE�� ���ִ� ���ǵ� EMPTY�� �ʱ�ȭ
											for(int i=0; i<29; i++) {
			 									for(int j=0; j<21; j++) {
													if(m_nGame[i][j] == ACTIVATE1 || m_nGame[i][j] == ACTIVATE2)
														m_nGame[i][j] = EMPTY;
												}
											}
											m_bEventItem[3] = TRUE;	// ��ź ������

											PlaySound(IDR_ITEM);
											//////////////////////////////////////////////////////////////////////
											m_bCurItem[4] = FALSE;
										}
									}
								}
								else if(m_nRand == 2) {
									if(m_bCurItem[0]) {
										if(nCurX==11 && nCurY==7) {
											//////////////////////////////////////////////////////////////////////
											m_nCurType = GetOtherType(m_nCurType);
											m_nCurType = GetOtherType(m_nCurType);
											m_nCurType = GetOtherType(m_nCurType);
											m_nCurType = GetOtherType(m_nCurType);
											m_nCurType = GetOtherType(m_nCurType);
											m_bEventItem[1] = TRUE;	// ���� ������

											PlaySound(IDR_ITEM);
											//////////////////////////////////////////////////////////////////////
											m_bCurItem[0] = FALSE;
										}
									}
									if(m_bCurItem[1]) {
										if(nCurX==15 && nCurY==9) {
											//////////////////////////////////////////////////////////////////////
											m_nCurType = GetOtherType(m_nCurType);
											m_nCurType = GetOtherType(m_nCurType);
											m_nCurType = GetOtherType(m_nCurType);
											m_nCurType = GetOtherType(m_nCurType);
											m_nCurType = GetOtherType(m_nCurType);
											m_bEventItem[1] = TRUE;	// ���� ������

											PlaySound(IDR_ITEM);
											//////////////////////////////////////////////////////////////////////
											m_bCurItem[1] = FALSE;
										}
									}
									if(m_bCurItem[2]) {
										if(nCurX==12 && nCurY==10) {
											//////////////////////////////////////////////////////////////////////
											m_nGame[nCurX][nCurY] = EMPTY;
											m_nGame[m_nOldX][m_nOldY] = m_nCurType;
											m_nStats = 0;
											m_bCheckMove = FALSE;

											// ACTIVATE�� ���ִ� ���ǵ� EMPTY�� �ʱ�ȭ
											for(int i=0; i<29; i++) {
			 									for(int j=0; j<21; j++) {
													if(m_nGame[i][j] == ACTIVATE1 || m_nGame[i][j] == ACTIVATE2)
														m_nGame[i][j] = EMPTY;
												}
											}
											m_bEventItem[3] = TRUE;	// ��ź ������

											PlaySound(IDR_ITEM);
											//////////////////////////////////////////////////////////////////////
											m_bCurItem[2] = FALSE;
										}
									}
									if(m_bCurItem[3]) {
										if(nCurX==9 && nCurY==11) {
											//////////////////////////////////////////////////////////////////////
											m_nCurType = GetOtherType(m_nCurType);
											m_bEventItem[0] = TRUE;	// �ٳ��� ������

											PlaySound(IDR_ITEM);
											//////////////////////////////////////////////////////////////////////
											m_bCurItem[3] = FALSE;
										}
									}
									if(m_bCurItem[4]) {
										if(nCurX==17 && nCurY==11) {
											//////////////////////////////////////////////////////////////////////
											m_nCurType = GetOtherType(m_nCurType);
											m_bEventItem[0] = TRUE;	// �ٳ��� ������

											PlaySound(IDR_ITEM);
											//////////////////////////////////////////////////////////////////////
											m_bCurItem[4] = FALSE;
										}
									}
								}
								else if(m_nRand == 3) {
									if(m_bCurItem[0]) {
										if(nCurX==14 && nCurY==8) {
											//////////////////////////////////////////////////////////////////////
											m_nCurType = GetOtherType(m_nCurType);
											m_bEventItem[0] = TRUE;	// �ٳ��� ������

											PlaySound(IDR_ITEM);
											//////////////////////////////////////////////////////////////////////
											m_bCurItem[0] = FALSE;
										}
									}
									if(m_bCurItem[1]) {
										if(nCurX==12 && nCurY==10) {
											//////////////////////////////////////////////////////////////////////
											m_nGame[nCurX][nCurY] = EMPTY;
											m_nGame[m_nOldX][m_nOldY] = m_nCurType;
											m_nStats = 0;
											m_bCheckMove = FALSE;

											// ACTIVATE�� ���ִ� ���ǵ� EMPTY�� �ʱ�ȭ
											for(int i=0; i<29; i++) {
			 									for(int j=0; j<21; j++) {
													if(m_nGame[i][j] == ACTIVATE1 || m_nGame[i][j] == ACTIVATE2)
														m_nGame[i][j] = EMPTY;
												}
											}
											m_bEventItem[3] = TRUE;	// ��ź ������

											PlaySound(IDR_ITEM);
											//////////////////////////////////////////////////////////////////////
											m_bCurItem[1] = FALSE;
										}
									}
									if(m_bCurItem[2]) {
										if(nCurX==14 && nCurY==10) {
											//////////////////////////////////////////////////////////////////////
											m_nCurType = GetOtherType(m_nCurType);
											m_bEventItem[0] = TRUE;	// �ٳ��� ������

											PlaySound(IDR_ITEM);
											//////////////////////////////////////////////////////////////////////
											m_bCurItem[2] = FALSE;
										}
									}
									if(m_bCurItem[3]) {
										if(nCurX==16 && nCurY==10) {
											//////////////////////////////////////////////////////////////////////
											m_nCurType = GetOtherType(m_nCurType);
											m_nCurType = GetOtherType(m_nCurType);
											m_nCurType = GetOtherType(m_nCurType);
											m_nCurType = GetOtherType(m_nCurType);
											m_nCurType = GetOtherType(m_nCurType);
											m_bEventItem[1] = TRUE;	// ���� ������

											PlaySound(IDR_ITEM);
											//////////////////////////////////////////////////////////////////////
											m_bCurItem[3] = FALSE;
										}
									}
									if(m_bCurItem[4]) {
										if(nCurX==14 && nCurY==12) {
											//////////////////////////////////////////////////////////////////////
											m_nCurType = GetOtherType(m_nCurType);
											m_bEventItem[0] = TRUE;	// �ٳ��� ������

											PlaySound(IDR_ITEM);
											//////////////////////////////////////////////////////////////////////
											m_bCurItem[4] = FALSE;
										}
									}
								}
							}

						// GameOver Check
						if(CheckGameOver()) {
							GameOver();
						}

						// Ÿ���� ���ϹǷ�
						m_bCheckType = TRUE;

						// ���������� �ѱ�
						m_nCurType = GetOtherType(m_nCurType);

					} else {
						// �ٸ� ������ ���� ��ҵǾ�����
						// �´��� Ȯ���ϱ� ���Ͽ� �޽����ڽ� ȣ��
						// AfxMessageBox("Ȯ��");
							
						// CClientDC dc(this);
						// CDC memDC;
						// memDC.CreateCompatibleDC(&dc);

						// CBitmap memBMP, *pOldBitmap;
						// memBMP.CreateCompatibleBitmap(&dc, 23, 23);

						// pOldBitmap = (CBitmap *)memDC.SelectObject(&m_bmpDlg);
						// dc.BitBlt(m_pt1.x-(23/2), m_pt1.y-(23/2), 23, 23, &memDC, m_pt1.x-(23/2), m_pt1.y-(23/2), SRCCOPY);

						// memDC.SelectObject(pOldBitmap);

						m_nGame[m_nOldX][m_nOldY] = m_nCurType;
						m_nStats = 0;
						m_bCheckMove = FALSE;

						// ACTIVATE�� ���ִ� ���ǵ� EMPTY�� �ʱ�ȭ
						for(i=0; i<29; i++) {
						 	for(j=0; j<21; j++) {
								if(m_nGame[i][j] == ACTIVATE1 || m_nGame[i][j] == ACTIVATE2)
									m_nGame[i][j] = EMPTY;
							}
						}

						// Sound Cancel
						PlaySound(IDR_CANCEL);
					}

					// ���� �� ��ġ�� �ƴϱ���.. �����?? �����غ���..
					// ACTIVATE�� ��� ���� ���Ѿ� �Ҷ��� ACTIVATE�� �ڸ���
					// ���� ���������̴�.

					// for(i=0; i<29; i++) {
					// 	for(j=0; j<21; j++) {
					// 		if(m_nGame[i][j] == ACTIVATE)
					//			m_nGame[i][j] = EMPTY;
					//	}
					// }
				} 
			}
		} 
	}
	CDialog::OnLButtonDown(nFlags, point);
}


void CPrinceGameDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	// ���콺 ����� ����Ʈ�� �ɹ������� ��� 
	// RealDrawObject() �Լ����� ����
	m_pt = point;

	CDialog::OnMouseMove(nFlags, point);
}

void CPrinceGameDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	// RealDrawObject();
	
	CDialog::OnLButtonUp(nFlags, point);
}

void CPrinceGameDlg::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_bCheckMove) {
		// �ٸ� ������ ���� ��ҵǾ�����
		// �´��� Ȯ���ϱ� ���Ͽ� �޽����ڽ� ȣ��
		// AfxMessageBox("Ȯ��");
		int i, j;

		m_bCheckMove = FALSE;
		m_nStats = 0;
		m_nGame[m_nOldX][m_nOldY] = m_nCurType;
		// RealDrawObject();

		// ACTIVATE�� ���ִ� ���ǵ� EMPTY�� �ʱ�ȭ
		for(i=0; i<29; i++) {
		 	for(j=0; j<21; j++) {
				if(m_nGame[i][j] == ACTIVATE1 || m_nGame[i][j] == ACTIVATE2)
					m_nGame[i][j] = EMPTY;
			}
		}

		// Sound Cancel
		PlaySound(IDR_CANCEL);
	}
	
	CDialog::OnRButtonDown(nFlags, point);
}

void CPrinceGameDlg::RealDrawObject()
{
	// �� �Լ����� ȭ���� �׷��ִ°Ͱ� �ִϸ��̼� 
	// ȿ���� �ش�. (���� ���۸� �̿�)
	CClientDC dc(this);
	CRect rect;
	GetClientRect(&rect);

	CDC memDC, memDCTemp;
	CBitmap *pOB1, *pOB2, memBmp;
	static int nAnimate1, nAnimate2, nAnimate3,
		nCount, nItemCount, nWaitCount, nStartCount, nOverCount, nFireCount;
	int i, j;

	memDC.CreateCompatibleDC(&dc);
	memDCTemp.CreateCompatibleDC(&dc);

	memBmp.CreateCompatibleBitmap(&dc, 830, 527);

	pOB1 = (CBitmap *)memDCTemp.SelectObject(&m_bmpDlg);
	pOB2 = (CBitmap *)memDC.SelectObject(&memBmp);
	
	memDCTemp.SelectObject(&m_bmpDlg);
	memDC.BitBlt(0,	0, 830, 527, &memDCTemp, 0,	0, SRCCOPY);

	for(i=0; i<29; i++) {
	 	for(j=0; j<21; j++) {

			if(m_nGame[i][j] == HUMAN_RED) {
				// ������ ���� RED ���� �Ѹ���

				if(j>=15 && j<=18) {		// ���� 1

					int nType = m_nGame[i][j];

					if(nType != OFFBOARD && nType != EMPTY && nType != ACTIVATE1 && nType != ACTIVATE2)
					{
						memDCTemp.SelectObject(&m_bmpMask[0]);
						memDC.BitBlt(m_rectArray[i][j].left+m_rectArray[i][j].Width()/2-(26/2),
							m_rectArray[i][j].top+m_rectArray[i][j].Height()/2-(27/2),
							26, 27, &memDCTemp, 0, 0, SRCAND);
						memDCTemp.SelectObject(&m_bmpRed[0]);
						memDC.StretchBlt(m_rectArray[i][j].left+m_rectArray[i][j].Width()/2-(26/2),
							m_rectArray[i][j].top+m_rectArray[i][j].Height()/2-(27/2),
							26, 27, &memDCTemp, 26*nAnimate1, 0, 26, 27, SRCPAINT);
					}

				} else if(j>=10 && j<=14) {	// ���� 2

					int nType = m_nGame[i][j];

					if(nType != OFFBOARD && nType != EMPTY && nType != ACTIVATE1 && nType != ACTIVATE2)
					{
						memDCTemp.SelectObject(&m_bmpMask[1]);
						memDC.BitBlt(m_rectArray[i][j].left+m_rectArray[i][j].Width()/2-(25/2),
							m_rectArray[i][j].top+m_rectArray[i][j].Height()/2-(24/2),
							25, 24, &memDCTemp, 0, 0, SRCAND);
						memDCTemp.SelectObject(&m_bmpRed[1]);
						memDC.StretchBlt(m_rectArray[i][j].left+m_rectArray[i][j].Width()/2-(25/2),
							m_rectArray[i][j].top+m_rectArray[i][j].Height()/2-(24/2),
							25, 24, &memDCTemp, 25*nAnimate1, 0, 25, 24, SRCPAINT);
					}

				} else if(j>=6 && j<=9) {	// ���� 3

					int nType = m_nGame[i][j];

					if(nType != OFFBOARD && nType != EMPTY && nType != ACTIVATE1 && nType != ACTIVATE2)
					{
						memDCTemp.SelectObject(&m_bmpMask[2]);
						memDC.BitBlt(m_rectArray[i][j].left+m_rectArray[i][j].Width()/2-(23/2),
							m_rectArray[i][j].top+m_rectArray[i][j].Height()/2-(23/2),
							23, 23, &memDCTemp, 0, 0, SRCAND);
						memDCTemp.SelectObject(&m_bmpRed[2]);
						memDC.StretchBlt(m_rectArray[i][j].left+m_rectArray[i][j].Width()/2-(23/2),
							m_rectArray[i][j].top+m_rectArray[i][j].Height()/2-(23/2),
							23, 23, &memDCTemp, 23*nAnimate1, 0, 23, 23, SRCPAINT);
					}

				} else if(j>=2 && j<=5) {	// ���� 4

					int nType = m_nGame[i][j];

					if(nType != OFFBOARD && nType != EMPTY && nType != ACTIVATE1 && nType != ACTIVATE2)
					{
						memDCTemp.SelectObject(&m_bmpMask[3]);
						memDC.BitBlt(m_rectArray[i][j].left+m_rectArray[i][j].Width()/2-(21/2),
							m_rectArray[i][j].top+m_rectArray[i][j].Height()/2-(21/2),
							21, 21, &memDCTemp, 0, 0, SRCAND);
						memDCTemp.SelectObject(&m_bmpRed[3]);
						memDC.StretchBlt(m_rectArray[i][j].left+m_rectArray[i][j].Width()/2-(21/2),
							m_rectArray[i][j].top+m_rectArray[i][j].Height()/2-(21/2),
							21, 21, &memDCTemp, 21*nAnimate1, 0, 21, 21, SRCPAINT);
					}

				}

			}
			else if(m_nGame[i][j] == HUMAN_YELLOW) {
				// ������ ���� YELLOW ���� �Ѹ���

				if(j>=15 && j<=18) {		// ���� 1

					int nType = m_nGame[i][j];

					if(nType != OFFBOARD && nType != EMPTY && nType != ACTIVATE1 && nType != ACTIVATE2)
					{
						memDCTemp.SelectObject(&m_bmpMask[0]);
						memDC.BitBlt(m_rectArray[i][j].left+m_rectArray[i][j].Width()/2-(26/2),
							m_rectArray[i][j].top+m_rectArray[i][j].Height()/2-(27/2),
							26, 27, &memDCTemp, 0, 0, SRCAND);
						memDCTemp.SelectObject(&m_bmpYellow[0]);
						memDC.StretchBlt(m_rectArray[i][j].left+m_rectArray[i][j].Width()/2-(26/2),
							m_rectArray[i][j].top+m_rectArray[i][j].Height()/2-(27/2),
							26, 27, &memDCTemp, 26*nAnimate2, 0, 26, 27, SRCPAINT);
					}

				} else if(j>=10 && j<=14) {	// ���� 2

					int nType = m_nGame[i][j];

					if(nType != OFFBOARD && nType != EMPTY && nType != ACTIVATE1 && nType != ACTIVATE2)
					{
						memDCTemp.SelectObject(&m_bmpMask[1]);
						memDC.BitBlt(m_rectArray[i][j].left+m_rectArray[i][j].Width()/2-(25/2),
							m_rectArray[i][j].top+m_rectArray[i][j].Height()/2-(24/2),
							25, 24, &memDCTemp, 0, 0, SRCAND);
						memDCTemp.SelectObject(&m_bmpYellow[1]);
						memDC.StretchBlt(m_rectArray[i][j].left+m_rectArray[i][j].Width()/2-(25/2),
							m_rectArray[i][j].top+m_rectArray[i][j].Height()/2-(24/2),
							25, 24, &memDCTemp, 25*nAnimate2, 0, 25, 24, SRCPAINT);
					}

				} else if(j>=6 && j<=9) {	// ���� 3

					int nType = m_nGame[i][j];

					if(nType != OFFBOARD && nType != EMPTY && nType != ACTIVATE1 && nType != ACTIVATE2)
					{
						memDCTemp.SelectObject(&m_bmpMask[2]);
						memDC.BitBlt(m_rectArray[i][j].left+m_rectArray[i][j].Width()/2-(23/2),
							m_rectArray[i][j].top+m_rectArray[i][j].Height()/2-(23/2),
							23, 23, &memDCTemp, 0, 0, SRCAND);
						memDCTemp.SelectObject(&m_bmpYellow[2]);
						memDC.StretchBlt(m_rectArray[i][j].left+m_rectArray[i][j].Width()/2-(23/2),
							m_rectArray[i][j].top+m_rectArray[i][j].Height()/2-(23/2),
							23, 23, &memDCTemp, 23*nAnimate2, 0, 23, 23, SRCPAINT);
					}

				} else if(j>=2 && j<=5) {	// ���� 4

					int nType = m_nGame[i][j];

					if(nType != OFFBOARD && nType != EMPTY && nType != ACTIVATE1 && nType != ACTIVATE2)
					{
						memDCTemp.SelectObject(&m_bmpMask[3]);
						memDC.BitBlt(m_rectArray[i][j].left+m_rectArray[i][j].Width()/2-(21/2),
							m_rectArray[i][j].top+m_rectArray[i][j].Height()/2-(21/2),
							21, 21, &memDCTemp, 0, 0, SRCAND);
						memDCTemp.SelectObject(&m_bmpYellow[3]);
						memDC.StretchBlt(m_rectArray[i][j].left+m_rectArray[i][j].Width()/2-(21/2),
							m_rectArray[i][j].top+m_rectArray[i][j].Height()/2-(21/2),
							21, 21, &memDCTemp, 21*nAnimate2, 0, 21, 21, SRCPAINT);
					}

				}
			}
			else if(m_nGame[i][j] == HUMAN_GREEN) {
				// ������ ���� GREEN ���� �Ѹ���

				if(j>=15 && j<=18) {		// ���� 1

					int nType = m_nGame[i][j];

					if(nType != OFFBOARD && nType != EMPTY && nType != ACTIVATE1 && nType != ACTIVATE2)
					{
						memDCTemp.SelectObject(&m_bmpMask[0]);
						memDC.BitBlt(m_rectArray[i][j].left+m_rectArray[i][j].Width()/2-(26/2),
							m_rectArray[i][j].top+m_rectArray[i][j].Height()/2-(27/2),
							26, 27, &memDCTemp, 0, 0, SRCAND);
						memDCTemp.SelectObject(&m_bmpGreen[0]);
						memDC.StretchBlt(m_rectArray[i][j].left+m_rectArray[i][j].Width()/2-(26/2),
							m_rectArray[i][j].top+m_rectArray[i][j].Height()/2-(27/2),
							26, 27, &memDCTemp, 26*nAnimate3, 0, 26, 27, SRCPAINT);
					}

				} else if(j>=10 && j<=14) {	// ���� 2

					int nType = m_nGame[i][j];

					if(nType != OFFBOARD && nType != EMPTY && nType != ACTIVATE1 && nType != ACTIVATE2)
					{
						memDCTemp.SelectObject(&m_bmpMask[1]);
						memDC.BitBlt(m_rectArray[i][j].left+m_rectArray[i][j].Width()/2-(25/2),
							m_rectArray[i][j].top+m_rectArray[i][j].Height()/2-(24/2),
							25, 24, &memDCTemp, 0, 0, SRCAND);
						memDCTemp.SelectObject(&m_bmpGreen[1]);
						memDC.StretchBlt(m_rectArray[i][j].left+m_rectArray[i][j].Width()/2-(25/2),
							m_rectArray[i][j].top+m_rectArray[i][j].Height()/2-(24/2),
							25, 24, &memDCTemp, 25*nAnimate3, 0, 25, 24, SRCPAINT);
					}

				} else if(j>=6 && j<=9) {	// ���� 3

					int nType = m_nGame[i][j];

					if(nType != OFFBOARD && nType != EMPTY && nType != ACTIVATE1 && nType != ACTIVATE2)
					{
						memDCTemp.SelectObject(&m_bmpMask[2]);
						memDC.BitBlt(m_rectArray[i][j].left+m_rectArray[i][j].Width()/2-(23/2),
							m_rectArray[i][j].top+m_rectArray[i][j].Height()/2-(23/2),
							23, 23, &memDCTemp, 0, 0, SRCAND);
						memDCTemp.SelectObject(&m_bmpGreen[2]);
						memDC.StretchBlt(m_rectArray[i][j].left+m_rectArray[i][j].Width()/2-(23/2),
							m_rectArray[i][j].top+m_rectArray[i][j].Height()/2-(23/2),
							23, 23, &memDCTemp, 23*nAnimate3, 0, 23, 23, SRCPAINT);
					}

				} else if(j>=2 && j<=5) {	// ���� 4

					int nType = m_nGame[i][j];

					if(nType != OFFBOARD && nType != EMPTY && nType != ACTIVATE1 && nType != ACTIVATE2)
					{
						memDCTemp.SelectObject(&m_bmpMask[3]);
						memDC.BitBlt(m_rectArray[i][j].left+m_rectArray[i][j].Width()/2-(21/2),
							m_rectArray[i][j].top+m_rectArray[i][j].Height()/2-(21/2),
							21, 21, &memDCTemp, 0, 0, SRCAND);
						memDCTemp.SelectObject(&m_bmpGreen[3]);
						memDC.StretchBlt(m_rectArray[i][j].left+m_rectArray[i][j].Width()/2-(21/2),
							m_rectArray[i][j].top+m_rectArray[i][j].Height()/2-(21/2),
							21, 21, &memDCTemp, 21*nAnimate3, 0, 21, 21, SRCPAINT);
					}

				}
			}
			else if(m_nGame[i][j] == HUMAN_CYAN) {
				// ������ ���� CYAN ���� �Ѹ���

				if(j>=15 && j<=18) {		// ���� 1

					int nType = m_nGame[i][j];

					if(nType != OFFBOARD && nType != EMPTY && nType != ACTIVATE1 && nType != ACTIVATE2)
					{
						memDCTemp.SelectObject(&m_bmpMask[0]);
						memDC.BitBlt(m_rectArray[i][j].left+m_rectArray[i][j].Width()/2-(26/2),
							m_rectArray[i][j].top+m_rectArray[i][j].Height()/2-(27/2),
							26, 27, &memDCTemp, 0, 0, SRCAND);
						memDCTemp.SelectObject(&m_bmpCyan[0]);
						memDC.StretchBlt(m_rectArray[i][j].left+m_rectArray[i][j].Width()/2-(26/2),
							m_rectArray[i][j].top+m_rectArray[i][j].Height()/2-(27/2),
							26, 27, &memDCTemp, 26*nAnimate1, 0, 26, 27, SRCPAINT);
					}

				} else if(j>=10 && j<=14) {	// ���� 2

					int nType = m_nGame[i][j];

					if(nType != OFFBOARD && nType != EMPTY && nType != ACTIVATE1 && nType != ACTIVATE2)
					{
						memDCTemp.SelectObject(&m_bmpMask[1]);
						memDC.BitBlt(m_rectArray[i][j].left+m_rectArray[i][j].Width()/2-(25/2),
							m_rectArray[i][j].top+m_rectArray[i][j].Height()/2-(24/2),
							25, 24, &memDCTemp, 0, 0, SRCAND);
						memDCTemp.SelectObject(&m_bmpCyan[1]);
						memDC.StretchBlt(m_rectArray[i][j].left+m_rectArray[i][j].Width()/2-(25/2),
							m_rectArray[i][j].top+m_rectArray[i][j].Height()/2-(24/2),
							25, 24, &memDCTemp, 25*nAnimate1, 0, 25, 24, SRCPAINT);
					}

				} else if(j>=6 && j<=9) {	// ���� 3

					int nType = m_nGame[i][j];

					if(nType != OFFBOARD && nType != EMPTY && nType != ACTIVATE1 && nType != ACTIVATE2)
					{
						memDCTemp.SelectObject(&m_bmpMask[2]);
						memDC.BitBlt(m_rectArray[i][j].left+m_rectArray[i][j].Width()/2-(23/2),
							m_rectArray[i][j].top+m_rectArray[i][j].Height()/2-(23/2),
							23, 23, &memDCTemp, 0, 0, SRCAND);
						memDCTemp.SelectObject(&m_bmpCyan[2]);
						memDC.StretchBlt(m_rectArray[i][j].left+m_rectArray[i][j].Width()/2-(23/2),
							m_rectArray[i][j].top+m_rectArray[i][j].Height()/2-(23/2),
							23, 23, &memDCTemp, 23*nAnimate1, 0, 23, 23, SRCPAINT);
					}

				} else if(j>=2 && j<=5) {	// ���� 4

					int nType = m_nGame[i][j];

					if(nType != OFFBOARD && nType != EMPTY && nType != ACTIVATE1 && nType != ACTIVATE2)
					{
						memDCTemp.SelectObject(&m_bmpMask[3]);
						memDC.BitBlt(m_rectArray[i][j].left+m_rectArray[i][j].Width()/2-(21/2),
							m_rectArray[i][j].top+m_rectArray[i][j].Height()/2-(21/2),
							21, 21, &memDCTemp, 0, 0, SRCAND);
						memDCTemp.SelectObject(&m_bmpCyan[3]);
						memDC.StretchBlt(m_rectArray[i][j].left+m_rectArray[i][j].Width()/2-(21/2),
							m_rectArray[i][j].top+m_rectArray[i][j].Height()/2-(21/2),
							21, 21, &memDCTemp, 21*nAnimate1, 0, 21, 21, SRCPAINT);
					}

				}
			}
			else if(m_nGame[i][j] == HUMAN_BLUE) {
				// ������ ���� BLUE ���� �Ѹ���

				if(j>=15 && j<=18) {		// ���� 1

					int nType = m_nGame[i][j];

					if(nType != OFFBOARD && nType != EMPTY && nType != ACTIVATE1 && nType != ACTIVATE2)
					{
						memDCTemp.SelectObject(&m_bmpMask[0]);
						memDC.BitBlt(m_rectArray[i][j].left+m_rectArray[i][j].Width()/2-(26/2),
							m_rectArray[i][j].top+m_rectArray[i][j].Height()/2-(27/2),
							26, 27, &memDCTemp, 0, 0, SRCAND);
						memDCTemp.SelectObject(&m_bmpBlue[0]);
						memDC.StretchBlt(m_rectArray[i][j].left+m_rectArray[i][j].Width()/2-(26/2),
							m_rectArray[i][j].top+m_rectArray[i][j].Height()/2-(27/2),
							26, 27, &memDCTemp, 26*nAnimate2, 0, 26, 27, SRCPAINT);
					}

				} else if(j>=10 && j<=14) {	// ���� 2

					int nType = m_nGame[i][j];

					if(nType != OFFBOARD && nType != EMPTY && nType != ACTIVATE1 && nType != ACTIVATE2)
					{
						memDCTemp.SelectObject(&m_bmpMask[1]);
						memDC.BitBlt(m_rectArray[i][j].left+m_rectArray[i][j].Width()/2-(25/2),
							m_rectArray[i][j].top+m_rectArray[i][j].Height()/2-(24/2),
							25, 24, &memDCTemp, 0, 0, SRCAND);
						memDCTemp.SelectObject(&m_bmpBlue[1]);
						memDC.StretchBlt(m_rectArray[i][j].left+m_rectArray[i][j].Width()/2-(25/2),
							m_rectArray[i][j].top+m_rectArray[i][j].Height()/2-(24/2),
							25, 24, &memDCTemp, 25*nAnimate2, 0, 25, 24, SRCPAINT);
					}

				} else if(j>=6 && j<=9) {	// ���� 3

					int nType = m_nGame[i][j];

					if(nType != OFFBOARD && nType != EMPTY && nType != ACTIVATE1 && nType != ACTIVATE2)
					{
						memDCTemp.SelectObject(&m_bmpMask[2]);
						memDC.BitBlt(m_rectArray[i][j].left+m_rectArray[i][j].Width()/2-(23/2),
							m_rectArray[i][j].top+m_rectArray[i][j].Height()/2-(23/2),
							23, 23, &memDCTemp, 0, 0, SRCAND);
						memDCTemp.SelectObject(&m_bmpBlue[2]);
						memDC.StretchBlt(m_rectArray[i][j].left+m_rectArray[i][j].Width()/2-(23/2),
							m_rectArray[i][j].top+m_rectArray[i][j].Height()/2-(23/2),
							23, 23, &memDCTemp, 23*nAnimate2, 0, 23, 23, SRCPAINT);
					}

				} else if(j>=2 && j<=5) {	// ���� 4

					int nType = m_nGame[i][j];

					if(nType != OFFBOARD && nType != EMPTY && nType != ACTIVATE1 && nType != ACTIVATE2)
					{
						memDCTemp.SelectObject(&m_bmpMask[3]);
						memDC.BitBlt(m_rectArray[i][j].left+m_rectArray[i][j].Width()/2-(21/2),
							m_rectArray[i][j].top+m_rectArray[i][j].Height()/2-(21/2),
							21, 21, &memDCTemp, 0, 0, SRCAND);
						memDCTemp.SelectObject(&m_bmpBlue[3]);
						memDC.StretchBlt(m_rectArray[i][j].left+m_rectArray[i][j].Width()/2-(21/2),
							m_rectArray[i][j].top+m_rectArray[i][j].Height()/2-(21/2),
							21, 21, &memDCTemp, 21*nAnimate2, 0, 21, 21, SRCPAINT);
					}

				}
			}
			else if(m_nGame[i][j] == HUMAN_PINK) {
				// ������ ���� PINK ���� �Ѹ���

				if(j>=15 && j<=18) {		// ���� 1

					int nType = m_nGame[i][j];

					if(nType != OFFBOARD && nType != EMPTY && nType != ACTIVATE1 && nType != ACTIVATE2)
					{
						memDCTemp.SelectObject(&m_bmpMask[0]);
						memDC.BitBlt(m_rectArray[i][j].left+m_rectArray[i][j].Width()/2-(26/2),
							m_rectArray[i][j].top+m_rectArray[i][j].Height()/2-(27/2),
							26, 27, &memDCTemp, 0, 0, SRCAND);
						memDCTemp.SelectObject(&m_bmpPink[0]);
						memDC.StretchBlt(m_rectArray[i][j].left+m_rectArray[i][j].Width()/2-(26/2),
							m_rectArray[i][j].top+m_rectArray[i][j].Height()/2-(27/2),
							26, 27, &memDCTemp, 26*nAnimate3, 0, 26, 27, SRCPAINT);
					}

				} else if(j>=10 && j<=14) {	// ���� 2

					int nType = m_nGame[i][j];

					if(nType != OFFBOARD && nType != EMPTY && nType != ACTIVATE1 && nType != ACTIVATE2)
					{
						memDCTemp.SelectObject(&m_bmpMask[1]);
						memDC.BitBlt(m_rectArray[i][j].left+m_rectArray[i][j].Width()/2-(25/2),
							m_rectArray[i][j].top+m_rectArray[i][j].Height()/2-(24/2),
							25, 24, &memDCTemp, 0, 0, SRCAND);
						memDCTemp.SelectObject(&m_bmpPink[1]);
						memDC.StretchBlt(m_rectArray[i][j].left+m_rectArray[i][j].Width()/2-(25/2),
							m_rectArray[i][j].top+m_rectArray[i][j].Height()/2-(24/2),
							25, 24, &memDCTemp, 25*nAnimate3, 0, 25, 24, SRCPAINT);
					}


				} else if(j>=6 && j<=9) {	// ���� 3

					int nType = m_nGame[i][j];

					if(nType != OFFBOARD && nType != EMPTY && nType != ACTIVATE1 && nType != ACTIVATE2)
					{
						memDCTemp.SelectObject(&m_bmpMask[2]);
						memDC.BitBlt(m_rectArray[i][j].left+m_rectArray[i][j].Width()/2-(23/2),
							m_rectArray[i][j].top+m_rectArray[i][j].Height()/2-(23/2),
							23, 23, &memDCTemp, 0, 0, SRCAND);
						memDCTemp.SelectObject(&m_bmpPink[2]);
						memDC.StretchBlt(m_rectArray[i][j].left+m_rectArray[i][j].Width()/2-(23/2),
							m_rectArray[i][j].top+m_rectArray[i][j].Height()/2-(23/2),
							23, 23, &memDCTemp, 23*nAnimate3, 0, 23, 23, SRCPAINT);
					}

				} else if(j>=2 && j<=5) {	// ���� 4

					int nType = m_nGame[i][j];

					if(nType != OFFBOARD && nType != EMPTY && nType != ACTIVATE1 && nType != ACTIVATE2)
					{
						memDCTemp.SelectObject(&m_bmpMask[3]);
						memDC.BitBlt(m_rectArray[i][j].left+m_rectArray[i][j].Width()/2-(21/2),
							m_rectArray[i][j].top+m_rectArray[i][j].Height()/2-(21/2),
							21, 21, &memDCTemp, 0, 0, SRCAND);
						memDCTemp.SelectObject(&m_bmpPink[3]);
						memDC.StretchBlt(m_rectArray[i][j].left+m_rectArray[i][j].Width()/2-(21/2),
							m_rectArray[i][j].top+m_rectArray[i][j].Height()/2-(21/2),
							21, 21, &memDCTemp, 21*nAnimate3, 0, 21, 21, SRCPAINT);
					}

				}
			}
		}
	}

	// ���콺 ����� ���� �̵�
	if(m_bCheckMove) {
		if(m_nCurType == HUMAN_RED) {

			if(m_nOldY>=15 && m_nOldY<=18) {
				memDCTemp.SelectObject(&m_bmpMask[0]);
				memDC.BitBlt(m_pt.x-(26/2), m_pt.y-(27/2), 26, 27, &memDCTemp, 0, 0, SRCAND);
				memDCTemp.SelectObject(&m_bmpRed[0]);
				memDC.BitBlt(m_pt.x-(26/2), m_pt.y-(27/2), 26, 27, &memDCTemp, 0, 0, SRCPAINT);
			}
			else if(m_nOldY>=10 && m_nOldY<=14) {
				memDCTemp.SelectObject(&m_bmpMask[1]);
				memDC.BitBlt(m_pt.x-(25/2), m_pt.y-(24/2), 25, 24, &memDCTemp, 0, 0, SRCAND);
				memDCTemp.SelectObject(&m_bmpRed[1]);
				memDC.BitBlt(m_pt.x-(25/2), m_pt.y-(24/2), 25, 24, &memDCTemp, 0, 0, SRCPAINT);
			}
			else if(m_nOldY>=6 && m_nOldY<=9) {
				memDCTemp.SelectObject(&m_bmpMask[2]);
				memDC.BitBlt(m_pt.x-(23/2), m_pt.y-(23/2), 23, 23, &memDCTemp, 0, 0, SRCAND);
				memDCTemp.SelectObject(&m_bmpRed[2]);
				memDC.BitBlt(m_pt.x-(23/2), m_pt.y-(23/2), 23, 23, &memDCTemp, 0, 0, SRCPAINT);
			}
			else if(m_nOldY>=2 && m_nOldY<=5) {
				memDCTemp.SelectObject(&m_bmpMask[3]);
				memDC.BitBlt(m_pt.x-(21/2), m_pt.y-(21/2), 21, 21, &memDCTemp, 0, 0, SRCAND);
				memDCTemp.SelectObject(&m_bmpRed[3]);
				memDC.BitBlt(m_pt.x-(21/2), m_pt.y-(21/2), 21, 21, &memDCTemp, 0, 0, SRCPAINT);
			}

		} else if(m_nCurType == HUMAN_YELLOW) {

			if(m_nOldY>=15 && m_nOldY<=18) {
				memDCTemp.SelectObject(&m_bmpMask[0]);
				memDC.BitBlt(m_pt.x-(26/2), m_pt.y-(27/2), 26, 27, &memDCTemp, 0, 0, SRCAND);
				memDCTemp.SelectObject(&m_bmpYellow[0]);
				memDC.BitBlt(m_pt.x-(26/2), m_pt.y-(27/2), 26, 27, &memDCTemp, 0, 0, SRCPAINT);
			}
			else if(m_nOldY>=10 && m_nOldY<=14) {
				memDCTemp.SelectObject(&m_bmpMask[1]);
				memDC.BitBlt(m_pt.x-(25/2), m_pt.y-(24/2), 25, 24, &memDCTemp, 0, 0, SRCAND);
				memDCTemp.SelectObject(&m_bmpYellow[1]);
				memDC.BitBlt(m_pt.x-(25/2), m_pt.y-(24/2), 25, 24, &memDCTemp, 0, 0, SRCPAINT);
			}
			else if(m_nOldY>=6 && m_nOldY<=9) {
				memDCTemp.SelectObject(&m_bmpMask[2]);
				memDC.BitBlt(m_pt.x-(23/2), m_pt.y-(23/2), 23, 23, &memDCTemp, 0, 0, SRCAND);
				memDCTemp.SelectObject(&m_bmpYellow[2]);
				memDC.BitBlt(m_pt.x-(23/2), m_pt.y-(23/2), 23, 23, &memDCTemp, 0, 0, SRCPAINT);
			}
			else if(m_nOldY>=2 && m_nOldY<=5) {
				memDCTemp.SelectObject(&m_bmpMask[3]);
				memDC.BitBlt(m_pt.x-(21/2), m_pt.y-(21/2), 21, 21, &memDCTemp, 0, 0, SRCAND);
				memDCTemp.SelectObject(&m_bmpYellow[3]);
				memDC.BitBlt(m_pt.x-(21/2), m_pt.y-(21/2), 21, 21, &memDCTemp, 0, 0, SRCPAINT);
			}

		} else if(m_nCurType == HUMAN_GREEN) {

			if(m_nOldY>=15 && m_nOldY<=18) {
				memDCTemp.SelectObject(&m_bmpMask[0]);
				memDC.BitBlt(m_pt.x-(26/2), m_pt.y-(27/2), 26, 27, &memDCTemp, 0, 0, SRCAND);
				memDCTemp.SelectObject(&m_bmpGreen[0]);
				memDC.BitBlt(m_pt.x-(26/2), m_pt.y-(27/2), 26, 27, &memDCTemp, 0, 0, SRCPAINT);
			}
			else if(m_nOldY>=10 && m_nOldY<=14) {
				memDCTemp.SelectObject(&m_bmpMask[1]);
				memDC.BitBlt(m_pt.x-(25/2), m_pt.y-(24/2), 25, 24, &memDCTemp, 0, 0, SRCAND);
				memDCTemp.SelectObject(&m_bmpGreen[1]);
				memDC.BitBlt(m_pt.x-(25/2), m_pt.y-(24/2), 25, 24, &memDCTemp, 0, 0, SRCPAINT);
			}
			else if(m_nOldY>=6 && m_nOldY<=9) {
				memDCTemp.SelectObject(&m_bmpMask[2]);
				memDC.BitBlt(m_pt.x-(23/2), m_pt.y-(23/2), 23, 23, &memDCTemp, 0, 0, SRCAND);
				memDCTemp.SelectObject(&m_bmpGreen[2]);
				memDC.BitBlt(m_pt.x-(23/2), m_pt.y-(23/2), 23, 23, &memDCTemp, 0, 0, SRCPAINT);
			}
			else if(m_nOldY>=2 && m_nOldY<=5) {
				memDCTemp.SelectObject(&m_bmpMask[3]);
				memDC.BitBlt(m_pt.x-(21/2), m_pt.y-(21/2), 21, 21, &memDCTemp, 0, 0, SRCAND);
				memDCTemp.SelectObject(&m_bmpGreen[3]);
				memDC.BitBlt(m_pt.x-(21/2), m_pt.y-(21/2), 21, 21, &memDCTemp, 0, 0, SRCPAINT);
			}

		} else if(m_nCurType == HUMAN_CYAN) {

			if(m_nOldY>=15 && m_nOldY<=18) {
				memDCTemp.SelectObject(&m_bmpMask[0]);
				memDC.BitBlt(m_pt.x-(26/2), m_pt.y-(27/2), 26, 27, &memDCTemp, 0, 0, SRCAND);
				memDCTemp.SelectObject(&m_bmpCyan[0]);
				memDC.BitBlt(m_pt.x-(26/2), m_pt.y-(27/2), 26, 27, &memDCTemp, 0, 0, SRCPAINT);
			}
			else if(m_nOldY>=10 && m_nOldY<=14) {
				memDCTemp.SelectObject(&m_bmpMask[1]);
				memDC.BitBlt(m_pt.x-(25/2), m_pt.y-(24/2), 25, 24, &memDCTemp, 0, 0, SRCAND);
				memDCTemp.SelectObject(&m_bmpCyan[1]);
				memDC.BitBlt(m_pt.x-(25/2), m_pt.y-(24/2), 25, 24, &memDCTemp, 0, 0, SRCPAINT);
			}
			else if(m_nOldY>=6 && m_nOldY<=9) {
				memDCTemp.SelectObject(&m_bmpMask[2]);
				memDC.BitBlt(m_pt.x-(23/2), m_pt.y-(23/2), 23, 23, &memDCTemp, 0, 0, SRCAND);
				memDCTemp.SelectObject(&m_bmpCyan[2]);
				memDC.BitBlt(m_pt.x-(23/2), m_pt.y-(23/2), 23, 23, &memDCTemp, 0, 0, SRCPAINT);
			}
			else if(m_nOldY>=2 && m_nOldY<=5) {
				memDCTemp.SelectObject(&m_bmpMask[3]);
				memDC.BitBlt(m_pt.x-(21/2), m_pt.y-(21/2), 21, 21, &memDCTemp, 0, 0, SRCAND);
				memDCTemp.SelectObject(&m_bmpCyan[3]);
				memDC.BitBlt(m_pt.x-(21/2), m_pt.y-(21/2), 21, 21, &memDCTemp, 0, 0, SRCPAINT);
			}

		} else if(m_nCurType == HUMAN_BLUE) {

			if(m_nOldY>=15 && m_nOldY<=18) {
				memDCTemp.SelectObject(&m_bmpMask[0]);
				memDC.BitBlt(m_pt.x-(26/2), m_pt.y-(27/2), 26, 27, &memDCTemp, 0, 0, SRCAND);
				memDCTemp.SelectObject(&m_bmpBlue[0]);
				memDC.BitBlt(m_pt.x-(26/2), m_pt.y-(27/2), 26, 27, &memDCTemp, 0, 0, SRCPAINT);
			}
			else if(m_nOldY>=10 && m_nOldY<=14) {
				memDCTemp.SelectObject(&m_bmpMask[1]);
				memDC.BitBlt(m_pt.x-(25/2), m_pt.y-(24/2), 25, 24, &memDCTemp, 0, 0, SRCAND);
				memDCTemp.SelectObject(&m_bmpBlue[1]);
				memDC.BitBlt(m_pt.x-(25/2), m_pt.y-(24/2), 25, 24, &memDCTemp, 0, 0, SRCPAINT);
			}
			else if(m_nOldY>=6 && m_nOldY<=9) {
				memDCTemp.SelectObject(&m_bmpMask[2]);
				memDC.BitBlt(m_pt.x-(23/2), m_pt.y-(23/2), 23, 23, &memDCTemp, 0, 0, SRCAND);
				memDCTemp.SelectObject(&m_bmpBlue[2]);
				memDC.BitBlt(m_pt.x-(23/2), m_pt.y-(23/2), 23, 23, &memDCTemp, 0, 0, SRCPAINT);
			}
			else if(m_nOldY>=2 && m_nOldY<=5) {
				memDCTemp.SelectObject(&m_bmpMask[3]);
				memDC.BitBlt(m_pt.x-(21/2), m_pt.y-(21/2), 21, 21, &memDCTemp, 0, 0, SRCAND);
				memDCTemp.SelectObject(&m_bmpBlue[3]);
				memDC.BitBlt(m_pt.x-(21/2), m_pt.y-(21/2), 21, 21, &memDCTemp, 0, 0, SRCPAINT);
			}

		} else if(m_nCurType == HUMAN_PINK) {

			if(m_nOldY>=15 && m_nOldY<=18) {
				memDCTemp.SelectObject(&m_bmpMask[0]);
				memDC.BitBlt(m_pt.x-(26/2), m_pt.y-(27/2), 26, 27, &memDCTemp, 0, 0, SRCAND);
				memDCTemp.SelectObject(&m_bmpPink[0]);
				memDC.BitBlt(m_pt.x-(26/2), m_pt.y-(27/2), 26, 27, &memDCTemp, 0, 0, SRCPAINT);
			}
			else if(m_nOldY>=10 && m_nOldY<=14) {
				memDCTemp.SelectObject(&m_bmpMask[1]);
				memDC.BitBlt(m_pt.x-(25/2), m_pt.y-(24/2), 25, 24, &memDCTemp, 0, 0, SRCAND);
				memDCTemp.SelectObject(&m_bmpPink[1]);
				memDC.BitBlt(m_pt.x-(25/2), m_pt.y-(24/2), 25, 24, &memDCTemp, 0, 0, SRCPAINT);
			}
			else if(m_nOldY>=6 && m_nOldY<=9) {
				memDCTemp.SelectObject(&m_bmpMask[2]);
				memDC.BitBlt(m_pt.x-(23/2), m_pt.y-(23/2), 23, 23, &memDCTemp, 0, 0, SRCAND);
				memDCTemp.SelectObject(&m_bmpPink[2]);
				memDC.BitBlt(m_pt.x-(23/2), m_pt.y-(23/2), 23, 23, &memDCTemp, 0, 0, SRCPAINT);
			}
			else if(m_nOldY>=2 && m_nOldY<=5) {
				memDCTemp.SelectObject(&m_bmpMask[3]);
				memDC.BitBlt(m_pt.x-(21/2), m_pt.y-(21/2), 21, 21, &memDCTemp, 0, 0, SRCAND);
				memDCTemp.SelectObject(&m_bmpPink[3]);
				memDC.BitBlt(m_pt.x-(21/2), m_pt.y-(21/2), 21, 21, &memDCTemp, 0, 0, SRCPAINT);
			}
		} 
	}

	// ȭ�鿡 ������ ���� �� ǥ��
	if(m_nCurType == HUMAN_RED) {
		memDCTemp.SelectObject(&m_bmpMask[3]);
		memDC.BitBlt(678, 479, 21, 21, &memDCTemp, 0, 0, SRCAND);
		memDCTemp.SelectObject(&m_bmpRed[3]);
		memDC.StretchBlt(678, 479, 21, 21, &memDCTemp, 21*nAnimate1, 0, 21, 21, SRCPAINT);
	}
	else if(m_nCurType == HUMAN_YELLOW) {
		memDCTemp.SelectObject(&m_bmpMask[3]);
		memDC.BitBlt(678, 479, 21, 21, &memDCTemp, 0, 0, SRCAND);
		memDCTemp.SelectObject(&m_bmpYellow[3]);
		memDC.StretchBlt(678, 479, 21, 21, &memDCTemp, 21*nAnimate2, 0, 21, 21, SRCPAINT);
	}
	else if(m_nCurType == HUMAN_GREEN) {
		memDCTemp.SelectObject(&m_bmpMask[3]);
		memDC.BitBlt(678, 479, 21, 21, &memDCTemp, 0, 0, SRCAND);
		memDCTemp.SelectObject(&m_bmpGreen[3]);
		memDC.StretchBlt(678, 479, 21, 21, &memDCTemp, 21*nAnimate3, 0, 21, 21, SRCPAINT);
	}
	else if(m_nCurType == HUMAN_CYAN) {
		memDCTemp.SelectObject(&m_bmpMask[3]);
		memDC.BitBlt(678, 479, 21, 21, &memDCTemp, 0, 0, SRCAND);
		memDCTemp.SelectObject(&m_bmpCyan[3]);
		memDC.StretchBlt(678, 479, 21, 21, &memDCTemp, 21*nAnimate1, 0, 21, 21, SRCPAINT);
	}
	else if(m_nCurType == HUMAN_BLUE) {
		memDCTemp.SelectObject(&m_bmpMask[3]);
		memDC.BitBlt(678, 479, 21, 21, &memDCTemp, 0, 0, SRCAND);
		memDCTemp.SelectObject(&m_bmpBlue[3]);
		memDC.StretchBlt(678, 479, 21, 21, &memDCTemp, 21*nAnimate2, 0, 21, 21, SRCPAINT);
	}
	else if(m_nCurType == HUMAN_PINK) {
		memDCTemp.SelectObject(&m_bmpMask[3]);
		memDC.BitBlt(678, 479, 21, 21, &memDCTemp, 0, 0, SRCAND);
		memDCTemp.SelectObject(&m_bmpPink[3]);
		memDC.StretchBlt(678, 479, 21, 21, &memDCTemp, 21*nAnimate3, 0, 21, 21, SRCPAINT);
	}

	// ������ �߻��� �ִϸ��̼�
	if(m_bEventItem[0]) {	// �������� ���� ����
		if(m_bCheckItemCount)
			nItemCount = 0;
		memDCTemp.SelectObject(&m_bmpItemMask[0]);
		memDC.StretchBlt(m_rectArray[m_nCurX][m_nCurY].left+m_rectArray[m_nCurX][m_nCurY].Width()/2-(34/2),
			m_rectArray[m_nCurX][m_nCurY].top+m_rectArray[m_nCurX][m_nCurY].Height()/2-(34/2)-20,
			34, 34, &memDCTemp, 34*nItemCount, 0, 34, 34, SRCAND);
		memDCTemp.SelectObject(&m_bmpItem[0]);
		memDC.StretchBlt(m_rectArray[m_nCurX][m_nCurY].left+m_rectArray[m_nCurX][m_nCurY].Width()/2-(34/2),
			m_rectArray[m_nCurX][m_nCurY].top+m_rectArray[m_nCurX][m_nCurY].Height()/2-(34/2)-20,
			34, 34, &memDCTemp, 34*nItemCount, 0, 34, 34, SRCPAINT);
		m_bCheckItemCount = FALSE;
		if(nItemCount > 14) {
			m_bEventItem[0] = FALSE;
			m_bCheckItemCount = TRUE;
		}
	} 
	else if(m_bEventItem[1]) {	// �ѹ���
		if(m_bCheckItemCount)
			nItemCount = 0;
		memDCTemp.SelectObject(&m_bmpItemMask[1]);
		memDC.StretchBlt(m_rectArray[m_nCurX][m_nCurY].left+m_rectArray[m_nCurX][m_nCurY].Width()/2-(42/2),
			m_rectArray[m_nCurX][m_nCurY].top+m_rectArray[m_nCurX][m_nCurY].Height()/2-(34/2)-20,
			42, 34, &memDCTemp, 42*nItemCount, 0, 42, 34, SRCAND);
		memDCTemp.SelectObject(&m_bmpItem[1]);
		memDC.StretchBlt(m_rectArray[m_nCurX][m_nCurY].left+m_rectArray[m_nCurX][m_nCurY].Width()/2-(42/2),
			m_rectArray[m_nCurX][m_nCurY].top+m_rectArray[m_nCurX][m_nCurY].Height()/2-(34/2)-20,
			42, 34, &memDCTemp, 42*nItemCount, 0, 42, 34, SRCPAINT);
		m_bCheckItemCount = FALSE;
		if(nItemCount > 14) {
			m_bEventItem[1] = FALSE;
			m_bCheckItemCount = TRUE;
		}
	}
	else if(m_bEventItem[2]) {	// ��
		if(m_bCheckItemCount)
			nItemCount = 0;
		memDCTemp.SelectObject(&m_bmpItemMask[2]);
		memDC.StretchBlt(m_rectArray[m_nCurX][m_nCurY].left+m_rectArray[m_nCurX][m_nCurY].Width()/2-(46/2),
			m_rectArray[m_nCurX][m_nCurY].top+m_rectArray[m_nCurX][m_nCurY].Height()/2-(34/2)-20,
			46, 34, &memDCTemp, 46*nItemCount, 0, 46, 34, SRCAND);
		memDCTemp.SelectObject(&m_bmpItem[2]);
		memDC.StretchBlt(m_rectArray[m_nCurX][m_nCurY].left+m_rectArray[m_nCurX][m_nCurY].Width()/2-(46/2),
			m_rectArray[m_nCurX][m_nCurY].top+m_rectArray[m_nCurX][m_nCurY].Height()/2-(34/2)-20,
			46, 34, &memDCTemp, 46*nItemCount, 0, 46, 34, SRCPAINT);
		m_bCheckItemCount = FALSE;
		if(nItemCount > 14) {
			m_bEventItem[2] = FALSE;
			m_bCheckItemCount = TRUE;
		}
	}
	else if(m_bEventItem[3]) {	// �ʱ�ȭ
		if(m_bCheckItemCount)
			nItemCount = 0;
		memDCTemp.SelectObject(&m_bmpItemMask[3]);
		memDC.StretchBlt(m_rectArray[m_nCurX][m_nCurY].left+m_rectArray[m_nCurX][m_nCurY].Width()/2-(40/2),
			m_rectArray[m_nCurX][m_nCurY].top+m_rectArray[m_nCurX][m_nCurY].Height()/2-(34/2)-20,
			40, 34, &memDCTemp, 40*nItemCount, 0, 40, 34, SRCAND);
		memDCTemp.SelectObject(&m_bmpItem[3]);
		memDC.StretchBlt(m_rectArray[m_nCurX][m_nCurY].left+m_rectArray[m_nCurX][m_nCurY].Width()/2-(40/2),
			m_rectArray[m_nCurX][m_nCurY].top+m_rectArray[m_nCurX][m_nCurY].Height()/2-(34/2)-20,
			40, 34, &memDCTemp, 40*nItemCount, 0, 40, 34, SRCPAINT);
		m_bCheckItemCount = FALSE;
		if(nItemCount > 14) {
			m_bEventItem[3] = FALSE;
			m_bCheckItemCount = TRUE;
		}
	}

	// ���ð� �ִϸ��̼� 
	if(m_bCheckWaitAni) {
		if(m_bCheckWaitCount)
			nWaitCount = 0;
		memDCTemp.SelectObject(&m_bmpWaitMask);
		memDC.StretchBlt(830/2-(250/2), 527/2-(58/2),
			250, 58, &memDCTemp, 250*nWaitCount, 0, 250, 58, SRCAND);
		memDCTemp.SelectObject(&m_bmpWait);
		memDC.StretchBlt(830/2-(250/2), 527/2-(58/2),
			250, 58, &memDCTemp, 250*nWaitCount, 0, 250, 58, SRCPAINT);
		m_bCheckWaitCount = FALSE;
		if(nWaitCount > 5) {
			m_bCheckWaitAni = FALSE;
			m_bCheckWaitCount = TRUE;
		}
	}

	// ���ӿ��� �ִϸ��̼�
	if(m_bCheckGameOver) {
		if(m_bCheckOverCount)
			nOverCount = 0;
		memDCTemp.SelectObject(&m_bmpOverMask);
		memDC.StretchBlt(13, 15,
			802, 497, &memDCTemp, 802*nOverCount, 0, 802, 497, SRCAND);
		memDCTemp.SelectObject(&m_bmpOver);
		memDC.StretchBlt(13, 15,
			802, 497, &memDCTemp, 802*nOverCount, 0, 802, 497, SRCPAINT);
		m_bCheckOverCount = FALSE;
		if(nOverCount > 6) {
			m_bCheckGameOver = FALSE;
			m_bCheckOverCount = TRUE;
			m_bCheckFire = TRUE;
		}
	}

	// ���ӿ��� (Fire) �ִϸ��̼�
	if(m_bCheckFire) {
		if(m_bCheckFireCount)
			nFireCount = 0;
		memDCTemp.SelectObject(&m_bmpFire);
		memDC.StretchBlt(13, 15,
			802, 497, &memDCTemp, 802*nFireCount, 0, 802, 497, SRCCOPY);
		m_bCheckFireCount = FALSE;
		if(nFireCount > 18) {
			m_bCheckFire = FALSE;
			m_bCheckFireCount = TRUE;
			KillTimer(REALDRAW);
			KillTimer(WAITTIME);
			Sleep(1400);
			CPrinceGameDlg::OnOK();
		}
	}

	// ��ŸƮ �ִϸ��̼�
	if(m_bCheckStart) {
		if(m_bCheckStartCount)
			nStartCount = 0;
		memDCTemp.SelectObject(&m_bmpStartMask);
		memDC.StretchBlt(830/2-(580/2), 527/2-(300/2),
			580, 300, &memDCTemp, 580*nStartCount, 0, 580, 300, SRCAND);
		memDCTemp.SelectObject(&m_bmpStart);
		memDC.StretchBlt(830/2-(580/2), 527/2-(300/2),
			580, 300, &memDCTemp, 580*nStartCount, 0, 580, 300, SRCPAINT);
		m_bCheckStartCount = FALSE;
		if(nStartCount > 37) {
			m_bCheckStart = FALSE;
			m_bCheckStartCount = TRUE;
		}
	}

	// ȭ�鿡 �ѷ��ֱ�
	dc.BitBlt(0,0,rect.Width(),rect.Height(),&memDC,0,0,SRCCOPY);

	// �ִϸ��̼� ī���͸� ������Ŵ
	if(++nCount) { 
		++nAnimate1;
		if(nCount%2 == 0)
			++nItemCount;
		if(nCount%3 == 0)
			++nStartCount;
		if(nCount%4 == 0) {
			++nWaitCount;
			++nFireCount;
		}
		if(nCount%7 == 0)
			++nOverCount;

		if(nCount > 12)
			nAnimate1=0;
		if(nCount > 20)
			++nAnimate2;
		if(nCount > 32)
			nAnimate2=0;
		if(nCount > 40)
			++nAnimate3;
		if(nCount > 52)
			nAnimate3=0;
		if(nCount > 160)
			nCount = 0;
	}

	memDCTemp.SelectObject(pOB1);
	memDC.SelectObject(pOB2);
}

BOOL CPrinceGameDlg::CheckGameOver()
{
	// ���� �ο����� ���� Game Over üũ
	if(m_nPlayer == 2) {
		if (
		// RED ������ CYAN ��
		m_nGame[11][15] == HUMAN_CYAN  &&	m_nGame[13][15] == HUMAN_CYAN &&
		m_nGame[15][15] == HUMAN_CYAN  &&	m_nGame[17][15] == HUMAN_CYAN &&
		m_nGame[12][16] == HUMAN_CYAN  &&	m_nGame[14][16] == HUMAN_CYAN &&
		m_nGame[16][16] == HUMAN_CYAN  &&	m_nGame[13][17] == HUMAN_CYAN &&
		m_nGame[15][17] == HUMAN_CYAN  &&	m_nGame[14][18] == HUMAN_CYAN &&

		// CYAN ������ RED ��
		m_nGame[14][2] == HUMAN_RED    &&	m_nGame[13][3] == HUMAN_RED &&
		m_nGame[15][3] == HUMAN_RED    &&	m_nGame[12][4] == HUMAN_RED &&
		m_nGame[14][4] == HUMAN_RED    &&	m_nGame[16][4] == HUMAN_RED &&
		m_nGame[11][5] == HUMAN_RED    &&	m_nGame[13][5] == HUMAN_RED &&
		m_nGame[15][5] == HUMAN_RED    &&	m_nGame[17][5] == HUMAN_RED
		)
			return TRUE;
	}
	else if(m_nPlayer == 3) {
		if (
		// CYAN ������ RED ��
		m_nGame[14][2] == HUMAN_RED    &&	m_nGame[13][3] == HUMAN_RED &&
		m_nGame[15][3] == HUMAN_RED    &&	m_nGame[12][4] == HUMAN_RED &&
		m_nGame[14][4] == HUMAN_RED    &&	m_nGame[16][4] == HUMAN_RED &&
		m_nGame[11][5] == HUMAN_RED    &&	m_nGame[13][5] == HUMAN_RED &&
		m_nGame[15][5] == HUMAN_RED    &&	m_nGame[17][5] == HUMAN_RED &&

		// PINK ������ GREEN ��
		m_nGame[23][11] == HUMAN_GREEN &&	m_nGame[22][12] == HUMAN_GREEN &&
		m_nGame[24][12] == HUMAN_GREEN &&	m_nGame[21][13] == HUMAN_GREEN &&
		m_nGame[23][13] == HUMAN_GREEN &&	m_nGame[25][13] == HUMAN_GREEN &&
		m_nGame[20][14] == HUMAN_GREEN &&	m_nGame[22][14] == HUMAN_GREEN &&
		m_nGame[24][14] == HUMAN_GREEN &&	m_nGame[26][14] == HUMAN_GREEN &&

		// YELLOW ������ BLUE ��
		m_nGame[5][11] == HUMAN_BLUE   &&	m_nGame[4][12] == HUMAN_BLUE &&
		m_nGame[6][12] == HUMAN_BLUE   &&	m_nGame[3][13] == HUMAN_BLUE &&
		m_nGame[5][13] == HUMAN_BLUE   &&	m_nGame[7][13] == HUMAN_BLUE &&
		m_nGame[2][14] == HUMAN_BLUE   &&	m_nGame[4][14] == HUMAN_BLUE &&
		m_nGame[6][14] == HUMAN_BLUE   &&	m_nGame[8][14] == HUMAN_BLUE 
		)
			return TRUE;
	}
	else if(m_nPlayer == 4) {
		if(
		// RED ������ CYAN ��
		m_nGame[11][15] == HUMAN_CYAN  &&	m_nGame[13][15] == HUMAN_CYAN &&
		m_nGame[15][15] == HUMAN_CYAN  &&	m_nGame[17][15] == HUMAN_CYAN &&
		m_nGame[12][16] == HUMAN_CYAN  &&	m_nGame[14][16] == HUMAN_CYAN &&
		m_nGame[16][16] == HUMAN_CYAN  &&	m_nGame[13][17] == HUMAN_CYAN &&
		m_nGame[15][17] == HUMAN_CYAN  &&	m_nGame[14][18] == HUMAN_CYAN &&

		// CYAN ������ RED ��
		m_nGame[14][2] == HUMAN_RED    &&	m_nGame[13][3] == HUMAN_RED &&
		m_nGame[15][3] == HUMAN_RED    &&	m_nGame[12][4] == HUMAN_RED &&
		m_nGame[14][4] == HUMAN_RED    &&	m_nGame[16][4] == HUMAN_RED &&
		m_nGame[11][5] == HUMAN_RED    &&	m_nGame[13][5] == HUMAN_RED &&
		m_nGame[15][5] == HUMAN_RED    &&	m_nGame[17][5] == HUMAN_RED &&

		// GREEN ������ PINK ��
		m_nGame[2][6] == HUMAN_PINK    &&	m_nGame[4][6] == HUMAN_PINK &&
		m_nGame[6][6] == HUMAN_PINK    &&	m_nGame[8][6] == HUMAN_PINK &&
		m_nGame[3][7] == HUMAN_PINK    &&	m_nGame[5][7] == HUMAN_PINK &&
		m_nGame[7][7] == HUMAN_PINK    &&	m_nGame[4][8] == HUMAN_PINK &&
		m_nGame[6][8] == HUMAN_PINK    &&	m_nGame[5][9] == HUMAN_PINK &&

		// PINK ������ GREEN ��
		m_nGame[23][11] == HUMAN_GREEN &&	m_nGame[22][12] == HUMAN_GREEN &&
		m_nGame[24][12] == HUMAN_GREEN &&	m_nGame[21][13] == HUMAN_GREEN &&
		m_nGame[23][13] == HUMAN_GREEN &&	m_nGame[25][13] == HUMAN_GREEN &&
		m_nGame[20][14] == HUMAN_GREEN &&	m_nGame[22][14] == HUMAN_GREEN &&
		m_nGame[24][14] == HUMAN_GREEN &&	m_nGame[26][14] == HUMAN_GREEN
		)
			return TRUE;
	}
	else if(m_nPlayer == 6) {
		if(
		// RED ������ CYAN ��
		m_nGame[11][15] == HUMAN_CYAN  &&	m_nGame[13][15] == HUMAN_CYAN &&
		m_nGame[15][15] == HUMAN_CYAN  &&	m_nGame[17][15] == HUMAN_CYAN &&
		m_nGame[12][16] == HUMAN_CYAN  &&	m_nGame[14][16] == HUMAN_CYAN &&
		m_nGame[16][16] == HUMAN_CYAN  &&	m_nGame[13][17] == HUMAN_CYAN &&
		m_nGame[15][17] == HUMAN_CYAN  &&	m_nGame[14][18] == HUMAN_CYAN &&

		// CYAN ������ RED ��
		m_nGame[14][2] == HUMAN_RED    &&	m_nGame[13][3] == HUMAN_RED &&
		m_nGame[15][3] == HUMAN_RED    &&	m_nGame[12][4] == HUMAN_RED &&
		m_nGame[14][4] == HUMAN_RED    &&	m_nGame[16][4] == HUMAN_RED &&
		m_nGame[11][5] == HUMAN_RED    &&	m_nGame[13][5] == HUMAN_RED &&
		m_nGame[15][5] == HUMAN_RED    &&	m_nGame[17][5] == HUMAN_RED &&

		// GREEN ������ PINK ��
		m_nGame[2][6] == HUMAN_PINK    &&	m_nGame[4][6] == HUMAN_PINK &&
		m_nGame[6][6] == HUMAN_PINK    &&	m_nGame[8][6] == HUMAN_PINK &&
		m_nGame[3][7] == HUMAN_PINK    &&	m_nGame[5][7] == HUMAN_PINK &&
		m_nGame[7][7] == HUMAN_PINK    &&	m_nGame[4][8] == HUMAN_PINK &&
		m_nGame[6][8] == HUMAN_PINK    &&	m_nGame[5][9] == HUMAN_PINK &&

		// PINK ������ GREEN ��
		m_nGame[23][11] == HUMAN_GREEN &&	m_nGame[22][12] == HUMAN_GREEN &&
		m_nGame[24][12] == HUMAN_GREEN &&	m_nGame[21][13] == HUMAN_GREEN &&
		m_nGame[23][13] == HUMAN_GREEN &&	m_nGame[25][13] == HUMAN_GREEN &&
		m_nGame[20][14] == HUMAN_GREEN &&	m_nGame[22][14] == HUMAN_GREEN &&
		m_nGame[24][14] == HUMAN_GREEN &&	m_nGame[26][14] == HUMAN_GREEN &&

		// BLUE ������ YELLOW ��
		m_nGame[20][6] == HUMAN_YELLOW &&	m_nGame[22][6] == HUMAN_YELLOW &&
		m_nGame[24][6] == HUMAN_YELLOW &&	m_nGame[26][6] == HUMAN_YELLOW &&
		m_nGame[21][7] == HUMAN_YELLOW &&	m_nGame[23][7] == HUMAN_YELLOW &&
		m_nGame[25][7] == HUMAN_YELLOW &&	m_nGame[22][8] == HUMAN_YELLOW &&
		m_nGame[24][8] == HUMAN_YELLOW &&	m_nGame[23][9] == HUMAN_YELLOW &&

		// YELLOW ������ BLUE ��
		m_nGame[5][11] == HUMAN_BLUE   &&	m_nGame[4][12] == HUMAN_BLUE &&
		m_nGame[6][12] == HUMAN_BLUE   &&	m_nGame[3][13] == HUMAN_BLUE &&
		m_nGame[5][13] == HUMAN_BLUE   &&	m_nGame[7][13] == HUMAN_BLUE &&
		m_nGame[2][14] == HUMAN_BLUE   &&	m_nGame[4][14] == HUMAN_BLUE &&
		m_nGame[6][14] == HUMAN_BLUE   &&	m_nGame[8][14] == HUMAN_BLUE 
		)
			return TRUE;
	}

	// ���� �ο����� ���� �ʰų� ������ ���� �ʴٸ� FALSE ����
	return FALSE;
}

void CPrinceGameDlg::GameOver()
{
	// AfxMessageBox("������ �������ϴ�!!");
	m_bCheckGameOver = TRUE;

	// Sound Over
	PlaySound(IDR_OVER);

	// GameOver �ǰ��� ����â ������ ���� �������� �ű��
}

void CPrinceGameDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default

	switch(nIDEvent)
	{
	case REALDRAW: 
	 	RealDrawObject();
		break;
	case WAITTIME: 
	 	WaitTime();
		break;
	}

	CDialog::OnTimer(nIDEvent);
}

void CPrinceGameDlg::WaitTime()
{
	nWaitCount++;	// ��� ī��Ʈ ���� (1������ 1��)

	if(m_bCheckType) {
		// Type�� ���ϸ� ���ī��Ʈ �ʱ�ȭ
		nWaitCount = 0;
		m_bCheckType = FALSE;
	}
		
	if(nWaitCount == m_nWait) {
		// �ִϸ��̼� ����
		m_bCheckWaitAni = TRUE;

		// Sound Wait Change
		PlaySound(IDR_WAIT);

		if(m_bCheckMove) {
			m_nGame[m_nOldX][m_nOldY] = m_nCurType;
			m_nStats = 0;
			m_bCheckMove = FALSE;

			// ACTIVATE�� ���ִ� ���ǵ� EMPTY�� �ʱ�ȭ
			for(int i=0; i<29; i++) {
			 	for(int j=0; j<21; j++) {
					if(m_nGame[i][j] == ACTIVATE1 || m_nGame[i][j] == ACTIVATE2)
						m_nGame[i][j] = EMPTY;
				}
			}
		}
		m_nCurType = GetOtherType(m_nCurType);
		nWaitCount = 0;
	}
}