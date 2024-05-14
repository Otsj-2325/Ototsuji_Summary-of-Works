using System.Collections;
using System.Collections.Generic;
using UnityEditor.SearchService;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class SCR_StartButton : MonoBehaviour
{
    [SerializeField] string m_LoadSceneName;
    [SerializeField] SCR_FadeProccess cm_Fade; 

    Image m_Image;
    bool m_isChange = false;
    Color m_ImageColor;
    
    // Start is called before the first frame update
    void Start()
    {
        Application.targetFrameRate = 60;

        
    }

    private void Update()
    {
        ;
    }

    public void PlessProc(){
        cm_Fade.StartFadeOut(m_LoadSceneName);
    }
}
