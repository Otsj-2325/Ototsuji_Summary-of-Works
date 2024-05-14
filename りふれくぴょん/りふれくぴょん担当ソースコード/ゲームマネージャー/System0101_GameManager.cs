/**************************************************
 * update : 2022/ 04/ 03
 * written : otsj.
 * about : 
 * ゲームマネージャー
***************************************************/

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.InputSystem;
using UnityEngine.SceneManagement;
 public class System0101_GameManager : MonoBehaviour
{
    public static System0101_GameManager instance = null;
    public static System0101_GameManager Instance { get { return instance; } }
    [HideInInspector] public string ReleaseStageCounter = "ReleaseStageCounter";
    [HideInInspector] public string CollectCoinCounter = "CollectCoinCounter";
    [HideInInspector] public string PlayerHealthCounter = "PlayerHealthCounter";

    [SerializeField]private int GMMode = 0;
    [SerializeField]private bool resetGame = false;
    private bool isPaused = false;
    //0:Title   1:Select    5:Game     9:Result

    // Start is called before the first frame update
    void Awake()
    {
        if (instance != null && instance != this)
        {
            Destroy(this.gameObject);
            return;
        }
        instance = this;
        DontDestroyOnLoad(this.gameObject);

    }

    private void Update()
    {        
        
        if (resetGame)
        {
            System0000_GameMasterClass.instance.ResetGame();
            resetGame = false;
        }
        if (Gamepad.current == null)
        {
            return;
        }
        if (Gamepad.current.startButton.isPressed)
        {
            TogglePause();
        }       

        switch (GMMode)
        {
            case 0:
                TitleUpdate();
                break;

            case 1:
                SelectUpdate();
                break;

            case 5:
                GameUpdate();
                break;
 
            case 9:
                ResultUpdate();
                break;
        }
    }

    public void SetGMMode(int mode)
    {
        GMMode = mode;

    }
    public void TogglePause()
    {
        isPaused = !isPaused;
       
        //IsGamePaused = true;
        if (isPaused)
        {
           
           Time.timeScale = 0;
           Debug.Log("isPaused here in GM");
           
        }
 
    }
    public bool IsGamePaused
    {
        get { return isPaused; }
        set { isPaused = value; }
    }
    private void TitleUpdate()
    {

    }

    private void SelectUpdate()
    {

    }

    private void GameUpdate()
    {
        //BulletManager.CheckBulletList();

    }
 
    private void ResultUpdate()
    {

    }
}
