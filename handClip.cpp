// opencvtest.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include<vector>
#include "Labeling.h"
#include<iostream>
#include "handClip.h"


//short�l�摜image�ɂ����āAx���W��left��right�̊Ԃɂ���A��f�l��index�̉�f�̍ő�̍��������߂�
//index��1-255�܂ł�ΏۂƂ���B���̒l��������Ȃ��ꍇ0��Ԃ�
int calcMaxHeight(cv::Mat image, int left, int right, int index, int& top, int& bottom){
	int maxHeight = 0;
	top = image.rows;//�ł�����y���W�������l�Ƃ���
	bottom = 0;
	for(int x = left; x < right; x++){
		int start=0;
		int end =0;
		bool isStarted = false;
		for(int y = 0; y < image.rows; y++){
			//�ォ�珇�ԂɌ��Ă���
			unsigned char pixVal = image.data [y * image.step + 2 * image.channels() * x];//short�摜��ΏۂƂ��Ă���̂�2�������Ă���
			if(pixVal == index){
				if(!isStarted){
					start = y;
					end = y;
				    isStarted = true;
				}
				else{
					end = y;
				}
			}
		}

		if(isStarted){//index�Ɠ������s�N�Z���������maxHeight,top,bottom���X�V
			int height = end - start +1;
		    if(maxHeight < height){
			    maxHeight = height;
		    }
			if(top > start){
				top = start;
			}
			if(bottom < end){
				bottom = end;
			}
		}
	}
	return maxHeight;

}

// �K���ȐF���o���p
cv::RNG rnd(1192);
cv::Scalar randomColor()
{
    return cv::Scalar(rnd.next() & 0xFF, rnd.next() & 0xFF, rnd.next() & 0xFF);
}


//��̏������N���X

HandInfo::HandInfo()
{
}

HandInfo::HandInfo(int left, int top, int right, int bottom, bool isLeftPerson)
{
	handRect = cv::Rect(left, top, right-left, bottom-top);
	if(isLeftPerson){
		personLabel = 1;
	}
	else{
		personLabel = 2;
	}
}

bool HandInfo::isLeftPersonHand(){
	if(personLabel == 1){
		return true;
	}
	else{
		return false;
	}
}


int HandInfo::getLeft()
{
	return handRect.x;
}
	
int HandInfo::getRight()
{
	return handRect.x + handRect.width;
}


bool getHandsInfo(cv::Mat binImage, HandInfo& leftInfo, HandInfo& rightInfo)
{
	vector<HandInfo> leftHands;
	vector<HandInfo> rightHands;

	cv::Mat labelImage(binImage.size(), CV_16SC1);
 
    // ���x�����O�����{ �Q�l�������摜�ɑ΂��Ď��s����B
    LabelingBS  labeling;
    labeling.Exec(binImage.data, (short *)labelImage.data, binImage.cols, binImage.rows, true, 2000);  //�傫��2,000�ȉ��̃��x���͐؂�̂�
     

	for(int i=0; i< labeling.GetNumOfRegions();i++){//���ׂẴ��[�W�����ɑ΂��Ď��{

		Labeling<unsigned char,short>::RegionInfo *labelInfo = labeling.GetResultRegionInfo(i);

		float gx, gy;
		labelInfo->GetCenterOfGravity(gx, gy);

		int top1, bottom1, left1, right1;
		if( gx > binImage.cols / 2){//�d�S���������E�ɂ���ΉE�̐l�̎�Ƃ��ď���
			int top, left;
			labelInfo->GetMin(left, top);
			//�̈�̍�����U�Opix�̂Ƃ���܂ł��؂�̎�̗̈�Ƃ��āA���̍��������߂�
			int height = calcMaxHeight(labelImage, left, min(left+60,binImage.cols), i+1, top1, bottom1);
			left1 = left;
			right1=min((int)(left+height * 1.5), binImage.cols);//�����̂Q�{����̉����Ƃ���
			calcMaxHeight(labelImage, left1, right1, i+1, top1, bottom1);//���̗̈�ŏ㉺�̍ő�l�����߂�
			rightHands.push_back(HandInfo(left1, top1, right1, bottom1, false));
		}
		
		else{//�d�S��������荶�ɂ���΍��̐l�̎�Ƃ��ď���
			int right, bottom;
			labelInfo->GetMax(right, bottom);
			int height =calcMaxHeight(labelImage, max(right-60,0), right, i+1, top1,bottom1);
			left1 = max((int)(right - 1.5 * height), 0);
			right1 = right;
			calcMaxHeight(labelImage, left1, right1, i+1, top1, bottom1);
			leftHands.push_back(HandInfo(left1,top1, right1, bottom1, false));
		}
	}

	bool isLeftHandsExist = false;
    bool isRightHandsExist = false;

	//�����₪��������ꍇ�̑I��
	if(leftHands.size() > 0){
		leftInfo = leftHands[0];
		isLeftHandsExist = true;
		//���E�[���E�ɂ������c��
		for(int i=1; i < leftHands.size(); i++){
			if(leftHands[i].getRight() > leftInfo.getRight()){
				leftInfo = leftHands[i];
			}
		}
	}
	
	//�E���₪��������ꍇ�̑I��
	if(rightHands.size() > 0){
		rightInfo = rightHands[0];
		isRightHandsExist = true;
		//��荶�[�����ɂ������c��
		for(int i=1; i < rightHands.size(); i++){
			if(rightHands[i].getLeft() < rightHands[i].getRight()){
				rightInfo = rightHands[i];
			}
		}
	}

	return isLeftHandsExist && isRightHandsExist;
}
