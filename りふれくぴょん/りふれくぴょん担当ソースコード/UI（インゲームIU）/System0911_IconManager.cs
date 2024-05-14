using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class System0911_IconManager : MonoBehaviour
{
    [SerializeField] List<GameObject> coinIcons;
    [SerializeField] List<GameObject> healthIcons;
    [SerializeField] Character0106_ItemCollector itemCnt;
    int coinIconCnt = 3;
    int healthIconCnt = 6;

    int coinCnt = 0;
    int healthCnt = 0;

    // Start is called before the first frame update
    void Start()
    {
        coinIconCnt = coinIcons.Count;
        healthIconCnt = healthIcons.Count;

        coinCnt = itemCnt.GetGCnt();
        healthCnt = PlayerPrefs.GetInt(System0101_GameManager.instance.PlayerHealthCounter);
        Debug.Log("‚±‚¢‚ñ" + coinCnt);

    }

    // Update is called once per frame
    void Update()
    {

        if (coinCnt != itemCnt.GetGCnt())
        {
            coinCnt = itemCnt.GetGCnt();
            Debug.Log("‚±‚¢‚ñ"+coinCnt);
            for(int i = 0; i < coinIconCnt; i++)
            {
                GameObject icon = coinIcons[i];
                icon.SetActive(false);

                if (i < coinCnt)
                {
                    icon.SetActive(true);
                }
            }
        }

        if (healthCnt != PlayerPrefs.GetInt(System0101_GameManager.instance.PlayerHealthCounter))
        {
            healthCnt = PlayerPrefs.GetInt(System0101_GameManager.instance.PlayerHealthCounter);
            Debug.Log("UIƒ‰ƒCƒt" + healthCnt);
            for (int i = healthIconCnt - 1; i >= 0; i--)
            {
                //GameObject icon =healthIcons[i];
                healthIcons[i].SetActive(true);

                if (i > healthCnt - 1)
                {
                    healthIcons[i].SetActive(false);
                }
            }
        }
    }
}
