#ifndef CAMERA_H
#define CAMERA_H

#include "opencv2/opencv.hpp"

//�ő�l��ݒ肷��ϐ��Ƃ���
const unsigned short c_number_of_horizontal_pixel_max = 1920;//752;
const unsigned short c_number_of_vertical_pixel_max = 1080;//480;

const unsigned short c_delta_x = 1;
const unsigned short c_delta_y = 1;
const unsigned short c_index_num = 255;
const unsigned short c_pixcel_value_max = 255;
const unsigned short c_integral_thresh = 200;

struct illuminant
{
	double x;//X���W
	double y;//Y���W
	unsigned short number_of_used_pixcel;//���W�Z�o�ɗp����ꂽ�s�N�Z����
	unsigned short number_of_saturated_pixcel;//���W�Z�o�ɗp����ꂽ�s�N�Z���ŋP�x�l���O�a���Ă�����̂̐�
	double max_pixcel_intensity;//�ō��P�x
	double pixcel_total_intensity;//�P�x�ώZ�l
	unsigned short width;//����
	unsigned short length;//�c��
	unsigned short max_y;
	unsigned short min_y;
};


class CameraImage{
private :
	
	
	//��f�̏o�͒l
	static unsigned long s_sencer_output[c_number_of_vertical_pixel_max][c_number_of_horizontal_pixel_max];
	
	//�w�i�o�͂�������������f�̏o�͒l
	static unsigned long s_sencer_output_backgroundless[c_number_of_vertical_pixel_max][c_number_of_horizontal_pixel_max];

	//�c�ݕ␳��̃J�������W
	static double s_undistort_output_x[c_number_of_vertical_pixel_max][c_number_of_horizontal_pixel_max];
	static double s_undistort_output_y[c_number_of_vertical_pixel_max][c_number_of_horizontal_pixel_max];

	//���ӕύX�ɑ΂���P�x�␳��̉摜�̔Z�x�l
	static double s_vignetting_reduction_intensity[c_number_of_vertical_pixel_max][c_number_of_horizontal_pixel_max];
	static double s_vignetting_reduction_intensity_backgroundless[c_number_of_vertical_pixel_max][c_number_of_horizontal_pixel_max];
	
	//��f���L�����ۂ�
	static bool is_valid_pixel[c_number_of_vertical_pixel_max][c_number_of_horizontal_pixel_max];
	
	//��f���Ɏ����Ă��郉�x���ԍ�
	static unsigned short label[c_number_of_vertical_pixel_max][c_number_of_horizontal_pixel_max];
	
	//���x���̃J�E���g�l
	unsigned short label_count;
	
	//���x���ԍ����o���Ă������߂̃C���f�b�N�X
	unsigned short label_index[c_index_num];
	
	//��f����肤�郉�x���ԍ��̍ő�l
	unsigned short label_index_max;

	//������
	illuminant image[c_index_num];

	//�L���s�N�Z����臒l
	unsigned short integral_thresh;

	//���͉摜�̐��������̉�f��
	unsigned short number_of_horizontal_pixel;
	
	//���͉摜�̐��������̉�f��
	unsigned short number_of_vertical_pixel;


public :
	CameraImage();
	~CameraImage();
	void initialize();

	
	//�����摜�p�̈�̏�����
	void initImage();
	
	//�w��T�C�Y(�����~����)�̃J������f��Ǎ���
	void readData(cv::Mat rv_input_data);
		
	//�w��T�C�Y(�����~����)��Ǎ��݃t�@�C���o��
	void readDataDebug(cv::Mat rv_input_data, std::ofstream* rv_output_file);
	
	//�w��T�C�Y(�����~����)�̃J������f(�w�i�o�͂Ƃ̍���)��Ǎ���
	void readBackgroundlessData(cv::Mat rv_input_data);
	
	//�w��T�C�Y(�����~����)�̃J������f(�w�i�o�͂Ƃ̍���)��Ǎ��݃t�@�C���o��
	void readBackgroundlessDataDebug(cv::Mat rv_input_data, std::ofstream* rv_output_file);
	
	//�w��T�C�Y(�����~����)�̉摜��臒l���傫����f�ɑ΂��Đϕ��l���v�Z
	double calcIntegral(std::ofstream* rv_output_file);
	
	//�w��T�C�Y(�����~����)�̉摜��臒l���傫����f�ɑ΂��Đϕ��l(�w�i�o�͂Ƃ̍���)���v�Z
	double calcIntegralBackgroundless(std::ofstream* rv_output_file);

	//臒l�����Z������Ԃŉ摜�̏d�S���W���擾
	illuminant* getCenterOfGravityBackgroundlessData(std::ofstream* rv_output_file);

	//�w��T�C�Y(�����~����)�̉摜��臒l���傫����f�ɑ΂���臒l�����Z���ďd�S���W���v�Z
	void calcFeatureQuantity(std::ofstream* rv_output_file);

	//�w��T�C�Y(�����~����)�̃J�����摜���Q�l������
	void digitalizeData(std::ofstream* rv_output_file);
	
	//�w���臒l���傫����f�����x�����O����
	void labelData(std::ofstream* rv_output_file);
	
	//���W�������ɕ��ёւ���
	void sortCoordinateAscendingOrder(illuminant* rv_data, unsigned short rv_num);

	//���W���s�N�Z���̑������ɕ��ёւ���
	void sortCoordinatePixelDecendingOrder(illuminant* rv_data, unsigned short rv_num);

	//�����̂̓����ʂ̊i�[��̑O��̓���ւ����s��
	void swapIlluminants(illuminant* rv_data1, illuminant* rv_data2);

	//���������̉�f���̃Z�b�^�[
	void setHorizontalPixel(unsigned short setValue);

	//���������̉�f���̃Z�b�^�[
	void setVerticalPixel(unsigned short setValue);

	//�c�ݕ␳��̔����摜���擾
	illuminant* getUndistortArea(std::ofstream* rv_output_file);

	//�P�x���z�ɑ΂���c�ݕ␳
	void calcUndistortArea(std::ofstream* rv_output_file);

	//���ӌ����ɑ΂���P�x�␳��̔����摜���擾
	illuminant* getReduceVignettingArea(std::ofstream* rv_output_file);

	//���ӌ����ɑ΂���P�x�␳
	void reduceVignetting(std::ofstream* rv_output_file);

	

};
#endif
	
