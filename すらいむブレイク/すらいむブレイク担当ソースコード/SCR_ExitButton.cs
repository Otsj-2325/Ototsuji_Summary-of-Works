using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class SCR_ExitButton : MonoBehaviour
{
    public void ExitGame(){
#if UNITY_EDITOR
            UnityEditor.EditorApplication.isPlaying = false;
#else
        Application.Quit();
#endif
    }

    public void LoadTitle(){
        SceneManager.LoadScene("Title");
    }
}
