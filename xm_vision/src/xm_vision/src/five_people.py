# -*- coding: utf-8 -*-
# 

# 检测人脸
import face_recognition
import cv2

# 读取图片并识别人脸
img = face_recognition.load_image_file("/home/ye/Vision/data/identify.jpg")
face_locations = face_recognition.face_locations(img)
print face_locations

# 调用opencv函数显示图片
img = cv2.imread("/home/ye/Vision/data/identify.jpg")
cv2.namedWindow("原图")
cv2.imshow("原图", img)


cv2.putText(img,str(-1),(100,100),cv2.FONT_HERSHEY_COMPLEX,1,(255,0,0),2)

# 遍历每个人脸，并标注,保存人脸，保存rect
file_out = open('/home/ye/Vision/data/people_identify/people_rect.txt','w')
faceNum = len(face_locations)
for i in range(0, faceNum):
    top =  face_locations[i][0]-5
    right =  face_locations[i][1]+5
    bottom = face_locations[i][2]+5
    left = face_locations[i][3]-5

    start = (left, top)
    end = (right, bottom)
    tmp = img[top:bottom,left:right]  
    cv2.imwrite("/home/ye/Vision/data/people_identify_all/"+str(i)+".jpg",tmp)
    file_out.write(str(top) + '\n')
    file_out.write(str(right) + '\n')
    file_out.write(str(bottom) + '\n')
    file_out.write(str(left) + '\n')

    color = (55,255,155)
    thickness = 3
    cv2.rectangle(img, start, end, color, thickness)

file_out.close()
# 显示识别结果
cv2.namedWindow("识别")
cv2.imshow("识别", img)

cv2.waitKey(1000)
cv2.destroyAllWindows()
