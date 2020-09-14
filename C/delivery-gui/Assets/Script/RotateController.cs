using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RotateController : MonoBehaviour
{
    //用于SmoothDamp的参数
    public float smoothTime = 0.1F;
    private Vector3 velocity = Vector3.zero;

    //向上移动目标点
    private Vector3 targetPosition;
    void Start()
    {
        //初始化target
        targetPosition = transform.position;
    }

    void Update()
    {
        transform.Rotate(new Vector3(0, 1, 0));
        transform.position = Vector3.SmoothDamp(transform.position, targetPosition, ref velocity, smoothTime);
    }
    public void Destroy()
    {
        //更新targetPosition，启动推出动画
        targetPosition = new Vector3(transform.position.x, transform.position.y + 2, transform.position.z);
    }
}
