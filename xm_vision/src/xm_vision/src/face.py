import face_recognition
import cv2

def recognize(identifier):

    # Load a sample picture and learn how to recognize it.
    eg_face_image = face_recognition.load_image_file('/home/ye/Vision/data/people_identify/' + identifier + '.jpg')
    eg_face_encoding = face_recognition.face_encodings(eg_face_image)

    file_out = open('/home/ye/Vision/data/people_identify/message.txt','w')

    if len(eg_face_encoding) == 0:
        file_out.write(str(-2) + '\n')
        file_out.write(str(-2) + '\n')
        file_out.write(str(-2) + '\n')
        file_out.write(str(-2) + '\n')
        file_out.close()
    
    eg_face_encoding = eg_face_encoding[0]
    # Initialize some variables
    face_locations = []
    face_encodings = []

    # Load scene
    scene = cv2.imread('/home/ye/Vision/data/people_identify/scene.jpg')

    # Resize frame of video to 1/4 size for faster face recognition processing
    small_scene = cv2.resize(scene, (0, 0), fx=0.5, fy=0.5)

    # Find all the faces and face encodings in the current scene
    face_locations = face_recognition.face_locations(small_scene)
    face_encodings = face_recognition.face_encodings(small_scene, face_locations)

    for face_encoding,face_location in zip(face_encodings, face_locations):
        # See if the face is a match for the known face(s)
        match = face_recognition.compare_faces([eg_face_encoding], face_encoding,0.4)

        if match[0]:
            file_out.write(str(face_location[0] * 2) + '\n')
            file_out.write(str(face_location[1] * 2) + '\n')
            file_out.write(str(face_location[2] * 2) + '\n')
            file_out.write(str(face_location[3] * 2) + '\n')
            file_out.close()
            return

    file_out.write(str(-1) + '\n')
    file_out.write(str(-1) + '\n')
    file_out.write(str(-1) + '\n')
    file_out.write(str(-1) + '\n')
    file_out.close()

if __name__=="__main__":
    file_in = open('/home/ye/Vision/data/people_identify/id.txt','r')
    line = file_in.read()
    recognize(line[0])
