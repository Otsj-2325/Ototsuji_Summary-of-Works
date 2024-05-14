using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class SCR_FadeProccess : MonoBehaviour
{
    [SerializeField] public string m_LoadSceneName;
    [SerializeField] Image cm_Fade;

    Button m_Button;
    Image m_Image;
    bool m_FadeOut = false;
    bool m_FadeIn = false;
    Color m_ImageColor;

    private void Start()
    {
        if (cm_Fade)
        {
            m_Image = cm_Fade.GetComponent<Image>();
            m_ImageColor = m_Image.color;
            m_Image.gameObject.SetActive(false);
        }

        StartFadeIn();
    }

    void Update(){
        if (m_FadeOut)
        {
            m_ImageColor.a += 0.01f;
            Debug.Log("Alpha : " + m_ImageColor.a);

            if (m_ImageColor.a > 1)
            {
                m_FadeOut = false;
                ChangeSnece();
            }

            m_Image.color = m_ImageColor;
        }

        if (m_FadeIn)
        {
            m_ImageColor.a -= 0.01f;
            Debug.Log("Alpha : " + m_ImageColor.a);

            if (m_ImageColor.a < 0)
            {
                m_FadeIn = false;
                gameObject.SetActive(false);
            }

            m_Image.color = m_ImageColor;
        }
    }

    public void StartFadeIn()
    {
        m_ImageColor = m_Image.color;
        m_FadeIn = true;
        m_FadeOut = false;
        m_Image.gameObject.SetActive(true);
        m_ImageColor.a = 1.0f;
        m_Image.color = m_ImageColor;

    }

    public void StartFadeOut(string sceneName)
    {
        m_ImageColor = m_Image.color;
        m_LoadSceneName = sceneName;
        m_FadeIn = false;
        m_FadeOut = true;
        gameObject.SetActive(true);
        m_ImageColor.a = 0.0f;
        m_Image.color = m_ImageColor;

    }

    private void ChangeSnece()
    {
        try{
            SceneManager.LoadScene(m_LoadSceneName);

        }
        catch{
            Debug.Log("Couldn`t load scene : " + m_LoadSceneName);

#if UNITY_EDITOR
            UnityEditor.EditorApplication.isPlaying = false;
#else
            Application.Quit();
#endif
        }
    }

}
