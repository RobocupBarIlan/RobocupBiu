#include "BallDetector.h"


BallDetector::BallDetector() {
	// TODO Auto-generated constructor stub

}

BallDetector::~BallDetector() {
	// TODO Auto-generated destructor stub
}



/*
 * This is the main method which is called by the Vision thread to detect a ball.
 * Returns - center of ball
 */

Point BallDetector::GetBallCenter()
{


	Mat frame, frame_hsv, frame_gray, field_mat, original_field_mat, whites_mat, field_space_mat,white_blobs_clone,white_blobs,white_bulbs_edges; //frame=origianl image. frame_hsv=image after transform to hsv. field_mat=only pixels in bounds of field's green after calibration are 0 (all the rest 255). whites_mat=only white pixels in image. field_space_mat=all pixels below bounding_horizontal_line (i.e -assumed to be field). candidates_mat=pixels which are not field and white and below bounding horizontal line.
	Mat hsv_channels[NUM_CHANNELS]; //Will contain all 3 HSV channels splitted.
	Mat bgr_channels[NUM_CHANNELS]; //Will contain all 3 BGR channels splitted.
	uchar field_min_hue, field_max_hue; //Will mark which pixel is a field pixel after calib.
	ushort bounding_horizontal_line; //Will mark the
	Point center;
	//resize(frame, frame, Size(640, 480));

	VideoCapture cap(0); // open the default camera
	if (!cap.isOpened())  // check if we succeeded
	{
		return Point(-1,-1);
	}
		//First we take 3 shots from the camera and perform averaging in order to get rid of some of the white noise in image. this will enhance the performance of the algorithm:
		Mat first_frame,second_frame;
		cap >> first_frame; // get a new frame from camera
		resize(first_frame, first_frame, Size(640, 480), 0, 0);
		cap >> second_frame; // get a new frame from camera
		resize(second_frame, second_frame, Size(640, 480), 0, 0);



		cap>>frame;
		resize(frame,frame,Size(640,480),0,0);
		frame=0.333333*frame+0.333333*first_frame+0.333333*second_frame;



		//Brighten frame:
		//frame=frame-10;
		//GaussianBlur(frame, frame, cv::Size(3, 3), 0,0);



		split(frame, bgr_channels); //Split to B,G,R channels so we can manipulate the image later on.

		//Reduce noise:
		GaussianBlur(bgr_channels[0], bgr_channels[0], Size(3, 3), 0.5, 0.5);
		GaussianBlur(bgr_channels[1], bgr_channels[1], Size(3, 3), 0.5, 0.5);
		GaussianBlur(bgr_channels[2], bgr_channels[2], Size(3, 3), 0.5, 0.5);

		//Unsharp - strengthen the edges:
		Mat blurred_frame;
		GaussianBlur(frame, blurred_frame, cv::Size(3, 3), 0,0);
		cv::addWeighted(frame, 1.7, blurred_frame, -0.7, 0, frame);

		merge(bgr_channels, NUM_CHANNELS, frame); //Merge the channels back to one matrix after manipulation.

		cvtColor(frame, frame_hsv, CV_BGR2HSV); //Convert original RGB representation to HSV representation.

		//cvtColor(frame,frame_gray,CV_BGR2GRAY);
		split(frame_hsv, hsv_channels); //Split to H,S,V channels so we can use the hue,saturation&value matrices more conviniently later on.
		BallDetector::FieldCalibration(hsv_channels[0], field_min_hue, field_max_hue);

		field_mat = Mat::zeros(frame.rows, frame.cols, CV_8UC1); //Generate a 1-channel matrix with size of original image (unsigned char). set all pixels to initail value 255=(11111111)_2
		whites_mat = Mat::zeros(frame.rows, frame.cols, CV_8UC1); //Generate a 1-channel matrix with size of original image (unsigned char).
		white_blobs = Mat::zeros(frame.rows, frame.cols, CV_8UC1);//Generate a 1-channel matrix with size of original image (unsigned char).
																  //Generate the field_mat and whites_mat:
		for (int i = 0; i < frame.rows; i++)
		{
			for (int j = 0; j < frame.cols; j++)
			{
				//Check saturation& value against bounds:
				if (hsv_channels[1].at<uchar>(i, j) <= MAX_WHITE_SATURATION && hsv_channels[2].at<uchar>(i, j) >= MIN_WHITE_VALUE)
				{
					whites_mat.at<uchar>(i, j) = 255; //If in range (i.e- white pixel) set to 11111111 in binary.
				}

				//Check hue value against bounds:
				if (hsv_channels[0].at<uchar>(i, j) >= field_min_hue && hsv_channels[0].at<uchar>(i, j) <= field_max_hue &&  hsv_channels[1].at<uchar>(i, j)>MIN_SATURATION_FIELD )
				{
					field_mat.at<uchar>(i, j) = 255; //If in range (i.e- field pixel) set to 0.
				}

			}
		}
		//inRange(frame_gray,Scalar(100),Scalar(255),whites_mat);
		medianBlur(whites_mat,whites_mat,7); //Remove salt&pepper noise
		imshow("whites",whites_mat);
		original_field_mat=field_mat.clone();
		imshow("original_field_mat", original_field_mat);
		waitKey(1);
		Mat structure_element_= getStructuringElement(MORPH_ELLIPSE, Size(STRUCTURE_ELEMENT_SIZE, STRUCTURE_ELEMENT_SIZE));
		erode(field_mat, field_mat, structure_element_); //First perform erosion to remove lone points and noise
		medianBlur(field_mat, field_mat, 7); //Median filter to remove small blocks of white in black areas and vise versa.
		structure_element_ = getStructuringElement(MORPH_ELLIPSE, Size(MIN_BALL_DIAMETER , MIN_BALL_DIAMETER ));
		//Perform closing to the field_mat with circular structure element  -so we get trid of lines!-
		dilate(field_mat, field_mat, structure_element_);
		erode(field_mat, field_mat,structure_element_);
		bitwise_not(field_mat, field_mat);
		bitwise_not(original_field_mat, original_field_mat);
		//Because we do bitwise_and (to get rid of candidate pixels) later on - perform bitwise not on the field_mat
		bitwise_and(field_mat,original_field_mat,field_mat);
		imshow("field_mat",field_mat);
		waitKey(1);
		structure_element_= getStructuringElement(MORPH_ELLIPSE, Size(STRUCTURE_ELEMENT_SIZE+2, STRUCTURE_ELEMENT_SIZE+2));
		dilate(field_mat,field_mat,structure_element_);
		erode(field_mat,field_mat,structure_element_);
		bitwise_not(original_field_mat, original_field_mat);
		//cvtColor(frame, frame_gray, CV_BGR2GRAY);
		//medianBlur(frame_gray, frame_gray, 11);
		//equalizeHist(frame_gray, frame_gray);
		//const uchar WHITE_THRESHOLD = 125;
		//inRange(frame_gray, WHITE_THRESHOLD, 255, whites_mat);


		BallDetector::CalculateBoundingHorizontalLine(field_mat, bounding_horizontal_line);
		 //Generate field_space_mat=all pixels below bounding_horizontal_line (i.e -assumed to be field) are 255=(11111111)_2 and 0 else.
		field_space_mat = Mat::zeros(frame.rows, frame.cols, CV_8UC1);
		for (uint i = frame.rows - 1; i > 0; i--)
		{
			if (i >= bounding_horizontal_line) //If below bounding_horizontal_line
			{
				for (uint j = 0; j < frame.cols; j++) //Set all values to 255 there:
				{
					field_space_mat.at<uchar>(i, j) = 255;
				}
			}
			else
			{
				break;
			}
		}
		imshow("field_space_mat",field_space_mat);

		/*
			NEW SECTION:
			Try and dilate the whites_mat in order to fill the holes in the ball's whites map. afterwards we perform bitwise_and with
		*/

		structure_element_ = getStructuringElement(MORPH_ELLIPSE, Size(MIN_BALL_DIAMETER , MIN_BALL_DIAMETER )); //TODO maybe try with MORPH_RECT instaead
		//Perform closing to the field_mat with circular structure element  -so we get trid of lines!-
		dilate(whites_mat, whites_mat, structure_element_);

		//Calculate the white_blobs which contains potential candidates to be the ball:
		bitwise_and(whites_mat, field_mat, white_blobs); //white pixel & not field pixel
		bitwise_and(white_blobs, field_space_mat, white_blobs);



		Mat labels;
		Mat centroids;
		Mat stats;

		imshow("white_blobs",white_blobs);
		waitKey(1);

		white_blobs_clone=white_blobs.clone(); //Copy white_blobs mat because we apply canny to it later on and still need the original one for counting white pixels...

		//Compute connected components of candidates_mat:
		connectedComponentsWithStats(white_blobs_clone, labels, stats, centroids, 8, CV_16U);

		Canny(white_blobs_clone, white_blobs_clone, 80, 40, 3, true);



		//Find best candidate out of the results in potential_circles if found potential circles:
		if (centroids.rows <= 1) //If no connected component found:
		{
			//No potential circle found:
		//	cout << "No ball in frame!(1)\n" << endl;
			//TODO set default values when ball not found for radii,center =-1,-1,-1.
			center=Point(-1,-1); //Return -1,-1 as ball not found.
		}
		else
		{
			double max_correlation = -INFINITY;
			uint white_pixels_counter;
			uint num_of_pixels_in_sub_image;
			double white_pixels_in_blob_ratio;
			double max_white_pixels_in_blob_ratio=0;
			uint index_of_max = 1;
			Mat candidate_sub_image; //We take the part of the candidates matrix which contains the connected component and calculate the histogram of gradients. then find best match to a circle histogram of gradients.
			double correlation = 0;
			int width_of_candidate=1,height_of_candidate=1;
			int top,left,width,height;
			//Find candidate with max Area to width*height of bounding box which does not have area large than PI*r_max^2 and less than PI*r_min^2:
			for (uint j = 1; j < centroids.rows; j++)
			{
				//If dimensions are proper:
				if (stats.at<int>(j, CC_STAT_AREA) <= PI*pow(MAX_BALL_DIAMETER / 2, 2) && stats.at<int>(j, CC_STAT_AREA) >= PI*pow(MIN_BALL_DIAMETER / 2, 2) && stats.at<int>(j, CC_STAT_WIDTH) <= MAX_BALL_DIAMETER &&stats.at<int>(j, CC_STAT_HEIGHT) <= MAX_BALL_DIAMETER && stats.at<int>(j, CC_STAT_WIDTH) >= MIN_BALL_DIAMETER &&stats.at<int>(j, CC_STAT_HEIGHT) >= MIN_BALL_DIAMETER)
				{
					if (stats.at<int>(j, CC_STAT_WIDTH) != 0 && stats.at<int>(j, CC_STAT_HEIGHT) != 0)
					{
						top=stats.at<int>(j, CC_STAT_TOP);
						left=stats.at<int>(j, CC_STAT_LEFT);
						width=stats.at<int>(j, CC_STAT_WIDTH)-1;
						height=stats.at<int>(j, CC_STAT_HEIGHT)-1;
						//Extend a bit the bounds if possible so we get enough black background for the candidate:
						if(top-CANDIDATE_SUB_IMAGE_CROP_EXTENSION>=1)
						{
							top=top-CANDIDATE_SUB_IMAGE_CROP_EXTENSION;
							if(top+height
									+2*CANDIDATE_SUB_IMAGE_CROP_EXTENSION<frame.rows-1)
								height=height+2*CANDIDATE_SUB_IMAGE_CROP_EXTENSION;
						}
						if(left-CANDIDATE_SUB_IMAGE_CROP_EXTENSION>=1)
						{
							left=left-CANDIDATE_SUB_IMAGE_CROP_EXTENSION;
							if(left+width
									+2*CANDIDATE_SUB_IMAGE_CROP_EXTENSION<frame.cols-1)
								width=width+2*CANDIDATE_SUB_IMAGE_CROP_EXTENSION;
						}
						BallDetector::CalculateCorrelationToCircleHist(white_blobs_clone(cv::Rect(left, top, width, height)), correlation );
						//BallDetector::CalculateCorrelationToCircleHist(white_blobs_clone(cv::Rect(max(1.0,stats.at<int>(j, CC_STAT_LEFT)-10.0), max(stats.at<int>(j, CC_STAT_TOP)-10.0,1.0), round(1.15*stats.at<int>(j, CC_STAT_WIDTH)-1), round(1.2*stats.at<int>(j, CC_STAT_HEIGHT)-1))), correlation );
						//After finding a potential circle - we want to check whether the encircled section of image has enough white inside it (because ball is 50% white approx. we would like the encircled space to have at least ~30%-40% whites [taking into count some color errors.

						/* TODO

							the part of counting white pixels is useless if we dilate the whites_mat first because the ball or any other object has more white pixels than there originally were - so consider removing it!
						*/
						//white_pixels_counter=0;
						//num_of_pixels_in_sub_image=stats.at<int>(j, CC_STAT_WIDTH)*stats.at<int>(j, CC_STAT_HEIGHT);
						//for(int i=stats.at<int>(j, CC_STAT_LEFT);i<=stats.at<int>(j, CC_STAT_LEFT)+stats.at<int>(j, CC_STAT_WIDTH)-1;i++)
						//{
						//	for(int k=stats.at<int>(j, CC_STAT_TOP);k<=stats.at<int>(j, CC_STAT_TOP)+stats.at<int>(j, CC_STAT_HEIGHT)-1;k++)
						//	{

							//	if( white_blobs.at<uchar>(k,i)==255 && original_field_mat.at<uchar>(k,i)!=255) //If white pixel
								//	white_pixels_counter++;
							//}
						//}
						//cout<<"white_pixels_counter"<<white_pixels_counter<<endl;

					//	white_pixels_in_blob_ratio=(white_pixels_counter+0.0)/num_of_pixels_in_sub_image;
						//cout<<"white_pixels_ratio:"<<white_pixels_in_blub_ratio<<endl;
						//correlation = correlation;/ sqrt(stats.at<int>(j, CC_STAT_AREA)); //Normalize correlation by area.
						if (correlation > max_correlation)// && white_pixels_in_blob_ratio>=MIN_WHITE_PIXELS_RATIO_IN_BLUB)
						{
							max_correlation = correlation;
							//max_white_pixels_in_blob_ratio=white_pixels_in_blob_ratio;
							index_of_max = j;
							width_of_candidate=stats.at<int>(j, CC_STAT_WIDTH);
							height_of_candidate=stats.at<int>(j, CC_STAT_HEIGHT);

						}
						//cout << "x= " << centroids.at<double>(j, 0) << "y= " << centroids.at<double>(j, 1) << " WIDTH=" << stats.at<int>(j, CC_STAT_WIDTH) << "HEIGHT=" << stats.at<int>(j, CC_STAT_HEIGHT) << " correlation:" << correlation << "\n" << endl;

					}
				}
			}

			//The code down here is only for demonstration:
			top=stats.at<int>(index_of_max, CC_STAT_TOP);
			left=stats.at<int>(index_of_max, CC_STAT_LEFT);
			width=stats.at<int>(index_of_max, CC_STAT_WIDTH)-1;
			height=stats.at<int>(index_of_max, CC_STAT_HEIGHT)-1;
			//Extend a bit the bounds if possible so we get enough black background for the candidate:
			if(top-CANDIDATE_SUB_IMAGE_CROP_EXTENSION>=1)
			{
				top=top-CANDIDATE_SUB_IMAGE_CROP_EXTENSION;
				if(top+height
						+2*CANDIDATE_SUB_IMAGE_CROP_EXTENSION<frame.rows-1)
					height=height+2*CANDIDATE_SUB_IMAGE_CROP_EXTENSION;
			}
			if(left-CANDIDATE_SUB_IMAGE_CROP_EXTENSION>=1)
			{
				left=left-CANDIDATE_SUB_IMAGE_CROP_EXTENSION;
				if(left+width
						+2*CANDIDATE_SUB_IMAGE_CROP_EXTENSION<frame.cols-1)
					width=width+2*CANDIDATE_SUB_IMAGE_CROP_EXTENSION;
			}
			imshow("blob",white_blobs_clone(cv::Rect(left, top, width, height)));
			waitKey(1);



			//cout<<"Top:"<<stats.at<int>(index_of_max, CC_STAT_TOP)<<endl;
			//cout<<"Left:"<<stats.at<int>(index_of_max,CC_STAT_LEFT)<<endl;
			//cout<<"Width:"<<stats.at<int>(index_of_max,CC_STAT_WIDTH)<<endl;
			//cout<<"Height:"<<stats.at<int>(index_of_max,CC_STAT_HEIGHT)<<endl;
			//cout<<"max_white_pixels_in_blub_ratio"<<max_white_pixels_in_blub_ratio<<endl;
			if (correlation == -INFINITY) //If no candidate found - declare no ball
			{
				//No potential circle found:
				//cout << "No ball in frame!(1)\n" << endl;
				//TODO set default values when ball not found for radii,center =-1,-1,-1.
				center=Point(-1,-1); //Return -1,-1 as ball not found.
			}
			else
			{
				//Get center of circle with max support (located first by HoughTransform by default):
			    center = Point(cvRound(centroids.at<double>(index_of_max, 0)), cvRound(centroids.at<double>(index_of_max, 1)));
				ushort radius;
				int top_left_x, top_left_y,width=round(width_of_candidate/1.5),height=round(height_of_candidate/1.5);//width = 2 * SUB_IMAGE_HALF_WIDTH, height = 2 * SUB_IMAGE_HALF_WIDTH; //Used as boundaries for sub-image croppped around the center.
				Mat sub_image; //Will contain the cropped sub-image containing the ball.

							   //Now - crop the image to get a sub-image(of square shape) *containing* the ball:
				top_left_x = max(0, cvRound(center.x - width)-1);
				top_left_y = max(0, cvRound(center.y - height)-1);
				//cout << "center_X" << center.x << " center_Y" << center.y;
				//Prevent assertion failure - index out of bounds:
				if (top_left_x + width > frame.cols - 1)
				{
					width = frame.cols - 1 - top_left_x;
				}
				if (top_left_y + height > frame.rows - 1)
				{
					height = frame.rows - 1 - top_left_y;
				}
				if (top_left_y < bounding_horizontal_line)
				{
					top_left_y = bounding_horizontal_line;
					height = min(frame.rows - 1 - top_left_y, height);
				}
				sub_image =  original_field_mat(Rect(top_left_x, top_left_y, min(frame.cols-top_left_x+0.0,2*width+0.0), min(frame.rows-top_left_y+0.0,2.5*height+0.0)));
				medianBlur(sub_image,sub_image,9);
				//structure_element_= getStructuringElement(MORPH_ELLIPSE, Size(MIN_BALL_DIAMETER/2, MIN_BALL_DIAMETER/2));
				//erode(sub_image, sub_image, structure_element_);
				//dilate(sub_image, sub_image,structure_element_);


															 //Apply CHT to sub_image to get circle
				vector<Vec3f> potential_circles; //Will hold the results of the algorithm.

												 //Histogram equalization:
												 //equalizeHist(sub_image, sub_image);
				Mat blurred_sub_image;
				//GaussianBlur(sub_image, blurred_sub_image, Size(3, 3), 0.5, 0.5);
				//sub_image = 2 * sub_image - blurred_sub_image; //Unsharp

				imshow("SUB_IMAGE", sub_image);
				waitKey(1);

				Canny(sub_image, sub_image, 1400, 1, 5, true);

				//Mat structure_element = getStructuringElement(MORPH_ELLIPSE, Size(3, 3)); //Anchor point is center by default.s
				//morphologyEx(sub_image, sub_image, MORPH_DILATE, structure_element);

				GaussianBlur(sub_image, sub_image, Size(5, 5), 2, 2);
				structure_element_ = getStructuringElement(MORPH_ELLIPSE, Size(STRUCTURE_ELEMENT_SIZE , STRUCTURE_ELEMENT_SIZE));
				//Perform closing to the field_mat with circular structure element  -so we get trid of lines!-
				dilate(sub_image, sub_image, structure_element_);

				imshow("SUB_IMAGE_CANNY", sub_image);
				waitKey(1);
				//imshow("test", sub_image);
				HoughCircles(sub_image, potential_circles, CV_HOUGH_GRADIENT, 3, 3, 10, 80, MIN_BALL_DIAMETER / 2, MAX_BALL_DIAMETER / 2); //225 threshold for CHT accumulator is used to get all possible strong enough boundaries.

																																		   //Canny(sub_image, sub_image, 300, 200, 3, 1);

																																		   //BallDetector::GradientCircleHoughTransform(sub_image, circles, 20, 65, 5);
				if (potential_circles.size() > 0)
				{
					//Fit the answer to the original image:
					center = Point(cvRound(potential_circles[0][0]) + top_left_x, cvRound(potential_circles[0][1]) + top_left_y);
					radius = cvRound(potential_circles[0][2]);


					cout << center.y << "," << center.x << ","<< radius << "\n" << endl; //Test
					//	clock_t end = clock(); //Time after execution.
					//	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC; //Convert from clocks to seconds
					//cout << "elapsed time: " << elapsed_secs << "\n" << potential_circles.size() << endl;
					// circle center
					circle(frame, center, 3, Scalar(0, 255, 0), -1, 8, 0);
					// circle outline
					circle(frame, center, radius, Scalar(0, 0, 255), 3, 8, 0);

				}
				else
				{
					//No potential circle found:
					//cout << "No ball in frame!(2)\n" << endl;
					//TODO set default values when ball not found for radii,center =-1,-1,-1.
					center=Point(-1,-1); //Return -1,-1 as ball not found.
				}
			}
		}
		imshow("RESULT", frame);
		waitKey(1);
		return center;

}


/*
	This method gets a black and white (i.e values 0/255) matrix where the circle candidate should be in white and background black.
	The method computes the correlation between the histogram of gradients of a circle to the candidate and returns the result in double& correlation.
*/

void BallDetector::CalculateCorrelationToCircleHist(Mat bw_mat, double& correlation)
{
	//static int call_id=0;
	//imwrite( "bw_mat"+to_string(call_id+144)+".png", bw_mat );
	//call_id++;
	//The following array contains the histogram of a circle with radius 100. we use this histogram to find best candidate to be the ball:
	const double CIRCLE_HOG[DEG_DISCRETIZATION] = { 2.40022239117365E-05,0.00127652394054866,0.00144512168786942,0.00158963385665636,0.00255304788109731,0.00221585368887176,0.00279390236401953,0.0014210361093356,0.00421493847335513,0.00144512168786942,0.00161371943519018,0.00773140168130936,0.00151737842347087,0.00163780501372399,0.00284207221867119,0.00163780501372399,0.00175823160397712,0.00132469509761629,0.0174859737256359,0.00108384191711005,0.00115609735029553,0.00122835408589699,0.002047255941551,0.00308292670159341,0.00166189059225781,0.00132469509761629,0.00168597486837566,0.00830945035645713,0.00168597486837566,0.00168597486837566,0.00156554827812254,0.00464847497971595,0.00120426850736317,0.00151737842347087,0.00317926771331271,0.0018304870371626,0.00289024337573883,0.00211951137473649,0.00211951137473649,0.00223993926740558,0.00250487802644564,0.00245670686937801,0.00163780501372399,0.00250487802644564,0.002047255941551,0.00122835408589699,0.00240853571231037,0.00207134152008482,0.00233628027912488,0.00238445143619252,0.00219176811033794,0.00216768253180412,0.00187865819423024,0.00240853571231037,0.00313109655624508,0.00156554827812254,0.00366097537674118,0.00171006044690948,0.00154146269958872,0.00513018264314441,0.00144512168786942,0.00180640276104475,0.0014210361093356,0.00888749903160489,0.00146920726640323,0.00134878067615011,0.00175823160397712,0.00308292670159341,0.0019509149298317,0.00118018292882935,0.000963414024440954,0.00125243836201484,0.0173655458329668,0.00139695053080178,0.00175823160397712,0.00163780501372399,0.00301066996599196,0.00166189059225781,0.00149329284493705,0.0073460350296002,0.00130060951908247,0.0014210361093356,0.00457621824411449,0.00187865819423024,0.00269756004988425,0.00211951137473649,0.00248079244791183,0.00154146269958872,0.00139695053080178,0.00144512168786942,0.00122835408589699,0.00139695053080178,0.00158963385665636,0.00132469509761629,0.00264939019523259,0.00202317036301718,0.00293841453280647,0.00122835408589699,0.00392591413578124,0.00180640276104475,0.00122835408589699,0.00715335300616159,0.00146920726640323,0.00168597486837566,0.00286615779720501,0.00171006044690948,0.00166189059225781,0.00151737842347087,0.0180399368222498,0.00115609735029553,0.00134878067615011,0.00115609735029553,0.00178231718251094,0.00349237762942042,0.00137286625468393,0.00154146269958872,0.00166189059225781,0.00932103553796572,0.00137286625468393,0.00163780501372399,0.00154146269958872,0.00515426691926226,0.0017341460254433,0.00158963385665636,0.0037814019669943,0.00166189059225781,0.00308292670159341,0.00221585368887176,0.0018304870371626,0.00248079244791183,0.00228810912205725,0.00243262129084419,0.00214359695327031,0.00219176811033794,0.00207134152008482,0.00122835408589699,0.00192682935129788,0.00197499920594955,0.00202317036301718,0.00238445143619252,0.00233628027912488,0.00223993926740558,0.00231219470059107,0.00255304788109731,0.00269756004988425,0.00156554827812254,0.00305884112305959,0.00180640276104475,0.00144512168786942,0.00546737683536996,0.00166189059225781,0.00154146269958872,0.00146920726640323,0.00888749903160489,0.0014210361093356,0.00168597486837566,0.00156554827812254,0.00298658438745814,0.00192682935129788,0.00120426850736317,0.00132469509761629,0.00122835408589699,0.0160408507353505,0.00125243836201484,0.00163780501372399,0.00154146269958872,0.00264939019523259,0.00158963385665636,0.00156554827812254,0.00710518184909396,0.0014210361093356,0.00139695053080178,0.00447987723239519,0.0014210361093356,0.00289024337573883,0.00219176811033794,0.00250487802644564,0.00146920726640323,0.00132469509761629,0.00151737842347087,2.40022239117365E-05,0.00151737842347087,0.00166189059225781,0.00130060951908247,0.00238445143619252,0.00216768253180412,0.00272164562841807,0.00187865819423024,0.00447987723239519,0.00161371943519018,0.00139695053080178,0.00753871835545478,0.00161371943519018,0.0014210361093356,0.00276981678548571,0.0017341460254433,0.0018304870371626,0.00122835408589699,0.016787497157819,0.00125243836201484,0.000963414024440954,0.00144512168786942,0.0017341460254433,0.00337195103916729,0.00115609735029553,0.0017341460254433,0.00127652394054866,0.0089838400433242,0.00156554827812254,0.00197499920594955,0.00130060951908247,0.00496158489582365,0.00171006044690948,0.00146920726640323,0.00361280421967354,0.00171006044690948,0.00257713345963113,0.00231219470059107,0.00226402354352343,0.00255304788109731,0.00238445143619252,0.00231219470059107,0.00231219470059107,0.00223993926740558,0.0019509149298317,0.00122835408589699,0.00219176811033794,0.00262530461669877,0.00226402354352343,0.00226402354352343,0.00272164562841807,0.00238445143619252,0.0018304870371626,0.00211951137473649,0.00279390236401953,0.00166189059225781,0.00334786546063347,0.00180640276104475,0.00154146269958872,0.00462438940118213,0.00149329284493705,0.00149329284493705,0.00161371943519018,0.00874298686281795,0.00144512168786942,0.00139695053080178,0.00166189059225781,0.00332377988209966,0.00161371943519018,0.00130060951908247,0.00125243836201484,0.00120426850736317,0.01856981434033,0.00132469509761629,0.00146920726640323,0.00178231718251094,0.00250487802644564,0.00161371943519018,0.00154146269958872,0.00739420618666784,0.00158963385665636,0.00163780501372399,0.00447987723239519,0.0014210361093356,0.00296250011134029,0.00219176811033794,0.00255304788109731,0.00151737842347087,0.00151737842347087,0.0011079261932279,2.40022239117365E-05,0.00175823160397712,0.00127652394054866,0.00156554827812254,0.00240853571231037,0.00245670686937801,0.00257713345963113,0.00154146269958872,0.00450396281092901,0.00144512168786942,0.00134878067615011,0.00797225486181561,0.00151737842347087,0.0014210361093356,0.00298658438745814,0.00175823160397712,0.00171006044690948,0.00156554827812254,0.0190756075822923,0.00118018292882935,0.00122835408589699,0.00134878067615011,0.00192682935129788,0.00327560872503202,0.00139695053080178,0.0014210361093356,0.00132469509761629,0.00910426663357732,0.00144512168786942,0.00168597486837566,0.00137286625468393,0.00503384032900914,0.00171006044690948,0.00163780501372399,0.0035646330626059,0.00151737842347087,0.00245670686937801,0.00272164562841807,0.00207134152008482,0.00211951137473649,0.00211951137473649,0.00248079244791183,0.00226402354352343,0.00192682935129788,0.00219176811033794,0.00122835408589699,0.00248079244791183,0.00219176811033794,0.00223993926740558,0.00276981678548571,0.00209542709861864,0.00231219470059107,0.00226402354352343,0.00219176811033794,0.00250487802644564,0.00137286625468393,0.00404634072603436,0.00156554827812254,0.00151737842347087,0.00493749931728983,0.00166189059225781,0.00185457261569642,0.00132469509761629,0.00891158461013871,0.00154146269958872,0.00125243836201484,0.00146920726640323,0.00305884112305959,0.00171006044690948,0.00139695053080178,0.00122835408589699,0.00125243836201484,0.0190515220037584,0.00122835408589699,0.00178231718251094,0.00171006044690948,0.00245670686937801,0.00156554827812254,0.00154146269958872,0.00797225486181561,0.00115609735029553,0.00207134152008482,0.00414268173775367,0.00154146269958872,0.00264939019523259,0.00272164562841807,0.00226402354352343,0.00178231718251094,0.00139695053080178,0.00151737842347087,0.00175823160397712 };
	//const double ENOUGH_WHITE_PIXELS_THRESHOLD = 0.1; //The percentage of white pixels in given mat to pass correlation test.
	int angles_in_shape[DEG_DISCRETIZATION];
	Mat sobel_x = (Mat_<char>(3, 3) << -3, 0, 3, -10, 0, 10, -3, 0, 3);
	Mat sobel_y = (Mat_<char>(3, 3) << -3, -10, -3, 0, 0, 0, 3, 10, 3);
	Mat x_gradient_mat, y_gradient_mat;
	uint angle;
	//uint num_white_pixels = 0;

	filter2D(bw_mat, x_gradient_mat, CV_64F, sobel_x, Point(-1, -1), 0); //Apply sobel to bw_mat and save result into x_gradient_mat (integers array).
	filter2D(bw_mat, y_gradient_mat, CV_64F, sobel_y, Point(-1, -1), 0); //Apply sobel to bw_mat and save result into x_gradient_mat (integers array).
	for (int i = 0; i < DEG_DISCRETIZATION; i++) //Initialize to all zeros
	{
		angles_in_shape[i] = 0;
	}

	double correlation_hog=0;
	double variance=0; //Used to tell if we have many different angles in candidate. a circle should have big variance.
	correlation = 0; //Initialize to 0 as we will sum into it.
    //For each pixel - compute angle and add the appropriate value to correlation:
	for (int i = 0; i < bw_mat.rows; i++)
	{
		for (int j = 0; j < bw_mat.cols; j++)
		{

			if (x_gradient_mat.at<double>(i, j) != 0 && (y_gradient_mat.at<double>(i, j) != 0)) //If 0 -ignore -the pixel has the same gradient as a vertical/horizontal line and we ignore such angles.
			{
				angle = cvRound(180 + (180 / PI)*atan2(y_gradient_mat.at<double>(i, j), x_gradient_mat.at<double>(i, j)));
				correlation_hog += CIRCLE_HOG[angle];
				if (angles_in_shape[angle] == 0)
					variance++;
				angles_in_shape[angle]++;
			}
		}
	}
		correlation = correlation_hog*variance;
		//cout << "correlation: " << correlation << "\n" << endl;
//const double LOGISTIC_REGRESSION_CLASSIFIER[LOGISTIC_REGRESSION_CLASSIFIER_LENGTH]={-0.732127364554446,0,0,0,3.06073551933341,0,-37.3155915446048,5.21587394578777,0,0,2.86413809118745,11.1184148513782,34.587514354546,10.1973684383978,-22.1890232162236,3.48715605507156,18.4591349122941,0,5.4381168892936,-1.37166858978392,-12.1719789531069,7.83687477143428,-38.990521855582,-1.22342195701761,-19.3323463568084,-23.3753666769407,12.5364380372131,-34.8291197028564,31.0454822525037,-4.1346539131035,-22.8533941476849,34.2689636937673,18.6816702347511,0,9.58513333126968,-15.5942567635614,6.98318224025324,11.8475549150782,-24.4738588163979,32.4664502466523,14.6792981716189,-28.9682907581724,-26.5078536416835,-32.0527260026267,-26.6662833958913,-22.2923252386795,-20.1584992551866,-20.0301827767149,-16.437270307868,-10.0995800446479,-21.8499728793152,-8.91762872369219,35.0820859567161,21.8021465787808,-21.2216277944868,31.0822447236223,21.1279887425489,0,0.213535814097007,-38.3296626624585,17.8972812589422,-27.9345883443139,-29.6724844721747,18.4273986051183,-12.9891649884669,0,5.20221473585882,11.3738408720777,-13.4087020158537,-22.0871223759619,-20.052068038126,-32.0416661138012,-41.900605446359,2.28545491629104,0,15.3643890364171,28.4876114346452,19.103122288857,-27.6592745341497,-23.2405840125252,-35.9311569853988,1.36151000102107,20.9255676210512,7.91631018721877,0.107266434943021,5.30195639841033,-19.2095686852188,13.5474706751886,-22.2329836270638,14.6965530146771,26.6426405177179,28.7216859616464,16.4772970937366,27.5048969471764,-18.3352146918141,31.4980981445757,-9.98232392123966,15.8762124721411,-5.35480021507794,0.317758740445262,-18.4059652137708,12.884495312032,-19.2651069664308,5.75418187906786,-24.1459677249722,-19.4109716024164,-23.8182190219826,-27.5748621269188,-19.3643417468721,-23.9065064972859,-25.2508883439533,-16.1685230345546,-18.220543380133,-20.9704384535513,-35.4224482944791,-8.03220045872294,-16.5019844413477,16.853818947834,-18.8865344125855,22.6899731787504,-15.1856733098477,-6.6179271913812,-27.709730291547,-20.2578747954258,-15.1840803857218,19.0622216190711,-19.0433515864447,-1.14397801332525,-15.1826239549788,0,13.738797716205,-0.727475572144538,-34.2862063242488,-3.21756406703259,-19.5370062908342,-26.6077079302995,-39.5894516211355,-1.68562310415317,23.189703721815,15.3371643898589,9.79382446579453,5.22339970046713,-21.6377971414706,-13.5366696183422,-20.1559017004757,-0.31918172518412,4.32119927524412,0,-4.35128571902186,-1.61518774233663,-33.8825204697038,9.15547504098705,-27.5155298246799,-32.2216841876832,28.3039355492733,-14.6671875766173,5.37562520740554,-10.2840923724531,-15.9423793397966,-3.28521892472927,-15.1825490979776,-18.1346115952265,7.10786988833277,15.1529437908325,-8.31449234175564,16.1368072572327,-20.1319040324915,16.2550615599938,-27.6037509910958,-29.0773524133157,-29.7011519944438,-34.5017048477494,-17.1583942595483,-31.0863726148272,-29.8355525461886,-15.182561444223,-18.0693031309175,-12.8329594823164,-18.6513908605887,4.77086875540249,0.653292895472931,30.6447586363269,-19.708156280469,2.16043056451926,-15.1831864214798,-33.6106741269939,-18.1831528183282,-23.2162046513789,-15.1825499334556,-26.0898612019294,-15.4794757262732,-15.1824870081323,-15.1815954966846,4.80121683150782,-14.4199451969056,-22.417105297084,-33.5809212874263,-26.6239583362367,-18.2870162024359,-26.7183205933892,-40.0454214416814,33.8423758264491,24.7300235194388,18.9962909121234,-2.9106917808564,9.26919132068441,-15.9939689858824,0.0842861124917683,-21.6753452258815,-11.0248379839151,-0.88413889982434,-15.0162794209319,-33.539408597069,8.53054423465029,-24.0097664009944,23.0588797072165,-21.8235810991505,30.8704164161563,13.5153133493975,4.25560551698971,-7.09961128032011,21.7485178258614,-15.1828721590592,14.5004900381433,-15.1826026433153,11.3576484772588,-36.3786525784639,-11.3468822857607,-23.8359720756869,-1.92500965624196,-24.7731001780956,-3.05588034658748,-30.1715102463,-24.1225033582821,-16.6532267425786,-16.0942710254051,-16.4438693878085,-16.7309085768292,-22.2070011139858,-15.6806244693285,-16.9304014200396,34.6527783126004,-8.94729343891962,-0.769499577489628,-12.265914034627,31.253015304598,-15.3344911768614,10.9909672992849,-13.9583029015549,14.1819444578351,-15.1840282831609,-15.1821332721783,-15.1831701122544,3.83160352574566,-15.1827051713464,-13.6029140475854,-15.183541585049,0,-15.0114345249367,11.9538763446077,-23.7569881188537,11.1257157004913,-20.5300205126646,-7.3898850757201,-43.1241124709738,14.5091736622984,0.317209352898992,10.2653058924411,7.84926953035967,-11.072066665797,-20.9353946503931,-33.8299814356566,-24.5942764560109,-4.99427196442638,12.9727384515981,3.43406522005449,7.56344708813993,-14.208088956418,-20.2579498094814,-1.15697032103778,-19.5280714225075,-9.81693870770002,27.9413555226845,-6.98511603954279,22.2375632686438,-31.073242033794,-24.5279486463187,-9.63412459657333,-15.1834280124838,9.27900130553892,17.1799849211679,18.543264204922,-13.7947680917447,0,-33.2499935398757,25.6875674435062,-40.5897989601385,-17.7859774917973,-22.2572384275444,-29.8841355822526,-22.5197760329641,-28.9260626149536,-32.0416342884252,-18.4004392163784,-17.7430604317266,21.1981514439654,15.8609154604446,30.7060754006833,30.4309478588546,26.0791628691226,-17.4879558661936,17.1914473759193,-14.0328135063611,-24.3320654150789,17.9701328818658,-18.8371387964899,8.21364982783412,-39.3218966409428,-21.629653430519,-15.1827951187918,-15.1832275520004,-29.1693441646571,-21.7404658393182,-18.6065879478615,-17.6556701863681,-19.6181162158009,-16.8745229829728,-23.6175981125918,-26.9944912412675,-21.6848311706673,-32.8392358033782,-23.9412247535628,-17.1810022755008,-18.7220687434953,-20.5086347891684,-26.3741723222136,-16.8377392642858,-39.5850314215566,-20.4117447247703,-31.7984538144199,-16.7112105119821,-28.5832856057448,-17.0349978250764,-16.3940377551529,-15.9628019714114,-25.767430668648,-23.1248076642193,-17.4465974952665,-15.1838016313843,-25.3026878607081,-16.251547356808,-15.1837625157719,-15.1826138156767,-22.3755866677882,-17.8989495261045,-24.6823242927901,-28.8903081879458,-34.0053094284801,-26.7379094647476,-17.3723480638263,-19.2527068144743,-19.3611825902657,-27.2050569817924,-31.660722438504,-21.27450379087,-29.0451230118752,-36.6663336242765,-23.1703806168809,-22.084095250384,-22.7570181576152,-19.7995781590487,-21.6096308531432,-15.2482396501285,-17.7831346620351,-16.3321530433896,-15.183192496302,-15.1822799661111,-27.1747479608388,-18.4456113422447,-15.4006428958344,-15.1826420062849,-17.3111769616615,-19.3932837180899,-15.18086359179,-15.1837383365265,4.44045773113642,-21.276063135566,-21.5075213547193,-28.1356061898296,-5.91650390260886,-21.0280211535473,-29.2903278696469,-40.717467827536,6.37986619635669,-24.7149856211752,-34.5914783212393,-1.89246474862842,-30.933872341103,-26.8125310428573,-40.7629352134177,-26.9737913826117,6.81463586330373,19.2753936234629,13.0140823060608,18.1120518257557,1.54960917825754,-17.5088501068796,-12.5848145250173,-18.3640402836976,34.2096873206896,31.6465197407102,1.83048259412766,6.95542702591651,-10.4079208343638,-15.5646049499864,-15.1837514089937,-13.0498802921661,33.6710570214208,1.27042977289341,13.7327591659089,0.92910020867609,25.5090024004834,-16.158263171485,9.48486944798238,-31.6449433306842,-15.8480680855356,-16.9653813983347,-15.9741064459378,-15.6509203818124,-21.9150851806984,-20.6180002077188,-16.4939847008733,-15.6216948071571,30.0961201216157,24.1987723367384,-0.613570941314412,9.67769628677388,19.2250024897245,-15.1840572228346,-7.13879765195308,-13.9604245198551,14.6900631119631,6.77160684147721,-15.1831365074633,-12.5463117464637,-15.1826932070938,-15.1836214402975,-15.1849805075639,-15.182579275234,0,-42.3624278275159,-40.9924660979266,-37.4933638167397,-41.9978968236068,-26.6246917205831,-37.2877925584007,-43.6441743457389,27.6058283903301,-35.5098595460671,-27.4638351449109,-25.3963528279705,-17.9001973635379,-17.6692209037539,-29.0870668420386,-26.144469655626,-16.7341345693938,-22.8517019902275,-34.9661319696154,-26.2433045382887,-21.6881409091963,-15.6569331100706,-20.2214715660458,-29.7787873640764,22.763188684754,-7.22279147666645,-15.1836748228485,-13.9598221350285,-15.1846017031464,-15.1827944216593,-15.1827856674642,-15.1821771752768,9.75330457792057,-27.6698488221913,-27.346048794191,-30.6312465053374,-43.1451390354109,-26.1220217786287,-31.2436210826075,-42.0718781366889,-17.5026828915342,-18.0047355804767,-19.3364368018224,-16.5177830117307,-21.9610766534394,-23.7259810274383,-19.4471629267557,-21.9635772457638,20.4548531059729,-15.183956241659,-15.1831563147473,-14.9476707786306,-8.39145662827615,-15.1841069298743,-15.1824402759968,-15.1823045869021,-12.9906982193682,-15.1842123866661,-15.1825909983915,-15.1832533028924,-15.1820141196714,-15.1826148966569,-15.1834346300187,-15.1822738084601};
//double visual_words[LOGISTIC_REGRESSION_CLASSIFIER_LENGTH];
//int visual_word_code,sum=0;
//double estimation;
////Initialize visual_words_array;
//for(int i=0;i<LOGISTIC_REGRESSION_CLASSIFIER_LENGTH;i++)
//{
//	visual_words[i]=0;
//}
//
////Run over whole image to get a count of visual words in b&w image:
//for(int i=0;i<bw_mat.rows-VISUAL_WORDS_BLOCK_SIZE; i++)
//{
//	for(int j=0;j<bw_mat.cols-VISUAL_WORDS_BLOCK_SIZE; j++)
//	{
//		visual_word_code=256*(bw_mat.at<int>(i,j)&1)+128*((bw_mat.at<int>(i,j+1)&1))+64*((bw_mat.at<int>(i,j+2)&1))+32*((bw_mat.at<int>(i+1,j)&1))+16*((bw_mat.at<int>(i+1,j+1)&1))+8*((bw_mat.at<int>(i+1,j+2)&1))+4*((bw_mat.at<int>(i+2,j)&1))+2*((bw_mat.at<int>(i+2,j+1)&1))+((bw_mat.at<int>(i+2,j+2)&1));
//		visual_words[visual_word_code+1]++; //Increment by 1 the occurences of this visual word.
//	}
//}
//
////Remove stop words such as blocks with only 1 point and vertical/horizontal lines - which don't add information.
//visual_words[1]=0; //Whole block is black.
//visual_words[2]=0; //Only one point in block.
//visual_words[3]=0; //Only one point in block.
//visual_words[5]=0; //Only one point in block.
//visual_words[9]=0; //Only one point in block.
//visual_words[17]=0; //Only one point in block.
//visual_words[33]=0; //Only one point in block.
//visual_words[65]=0; //Only one point in block.
//visual_words[129]=0; //Only one point in block.
//visual_words[257]=0; //Only one point in block.
//visual_words[74]=0; //vertical line.
//visual_words[147]=0; //vertical line.
//visual_words[293]=0; //vertical line.
//visual_words[8]=0; //horizontal line.
//visual_words[57]=0; //horizontal line.
//visual_words[449]=0; //horizontal line.
//
////Normalize the visual_words array to get features between 0 and 1:
//for(int i=1;i<LOGISTIC_REGRESSION_CLASSIFIER_LENGTH;i++)
//{
//	sum=sum+visual_words[i];
//}
//if(sum!=0) //otherwise we can't calculate correlation
//{
//	for(int i=1;i<LOGISTIC_REGRESSION_CLASSIFIER_LENGTH;i++)
//	{
//		visual_words[i]=visual_words[i]/(sum+0.0);
//		//cout<<visual_words[i]<<",";
//	}
//	//cout<<endl;
//
//	//Set the bias feature to 1:
//	visual_words[0]=1;
//
//
//	for(int i=0;i<LOGISTIC_REGRESSION_CLASSIFIER_LENGTH;i++)
//	{
//		estimation=estimation+visual_words[i]*LOGISTIC_REGRESSION_CLASSIFIER[i];
//	}
//	correlation=Sigmoid(estimation);
//	cout<<correlation<<endl;
//}
//else
//{
//	correlation=0;
//	return;
//}


}



/*This method calibrates the green field hue bounds. It takes the hue matrix and returns the- field_min_hue and field_max_hue.
	We create the histogram of hue for the given matrix and take PERCENTAGE_THRESHOLD (=90%) in the middle of the green spectrum -
	assuming values on the edges of the green spectrum might be noise.*/
void BallDetector::FieldCalibration(Mat& hue_matrix, uchar& field_min_hue, uchar& field_max_hue)
{
	int hue_histogram[HUE_DISCRETIZATION];
	int total_num_of_green_pixels=0; //Tells how many green pixel in total range of greens as specified by MIN_GREEN_HUE & MAX_GREEN_HUE.
	int sum_of_green_pixels_in_histogram; //Will count green pixels until we reach the threshold defined by PERCENTAGE_THRESHOLD.
	const double PERCENTAGE_THRESHOLD = 0.95; //Determines how many green pixels we get rid of. the larger the less.
	const int TOTAL_NUM_OF_PIXELS = hue_matrix.cols*hue_matrix.rows;
	const int NOT_ENOUGH_GREENS_IN_IMAGE_THRESHOLD = 0.2; //Heuristic. might be changed. if not enough green pixels we can't estimate the field!.
	//Initialize the hue histogram with zeros:
	for (uint i = 0; i < HUE_DISCRETIZATION; i++)
	{
		hue_histogram[i] = 0;
	}
	//generate hue histogram:
	for (uint i = 0; i < hue_matrix.rows; i++)
	{
		for (uint j = 0; j < hue_matrix.cols; j++)
		{
			hue_histogram[hue_matrix.at<uchar>(i, j)] = hue_histogram[hue_matrix.at<uchar>(i, j)] + 1;
		}
	}
	//Sum all pixels to get total_num_of_green_pixels.
	for (uint k = 0; k < HUE_DISCRETIZATION; k++)
	{
		total_num_of_green_pixels = total_num_of_green_pixels + hue_histogram[k];
	}
	if (total_num_of_green_pixels < NOT_ENOUGH_GREENS_IN_IMAGE_THRESHOLD*TOTAL_NUM_OF_PIXELS) //If not enough green pixels in image:
	{
		cout << "Not enough green pixels in image to do field green calibration!" << endl;
		//Return default bounds of green- we don't have enough data to estimate.
		field_min_hue = MIN_GREEN_HUE;
		field_max_hue = MAX_GREEN_HUE;
		return;
	}
	//Now that we have histogram and sum of green pixels- we check where PERCENTAGE_THRESHOLD (=90%)of the green pixels lie on the spectrum:
	int i = MIN_GREEN_HUE;
	sum_of_green_pixels_in_histogram = hue_histogram[i];
	while (sum_of_green_pixels_in_histogram / (total_num_of_green_pixels + 0.0) < (1 - PERCENTAGE_THRESHOLD) / 2)
	{
		i++;
		sum_of_green_pixels_in_histogram = sum_of_green_pixels_in_histogram + hue_histogram[i];
	}
	field_min_hue = i; //field min hue is found.
	i = MAX_GREEN_HUE;
	sum_of_green_pixels_in_histogram = hue_histogram[i];
	while (sum_of_green_pixels_in_histogram / (total_num_of_green_pixels + 0.0) < (1 - PERCENTAGE_THRESHOLD) / 2)
	{
		i--;
		sum_of_green_pixels_in_histogram = sum_of_green_pixels_in_histogram + hue_histogram[i];
	}
	field_max_hue = i; //field min hue is found.

	//cout << "field max " << int(field_max_hue)<<endl; //TEST
	//cout << "field min "<< int(field_min_hue)<<endl;  //TEST


}

/*This method calculates the bounding horizontal line of the field. i.e - it is the horizontal line which PERCENTAGE_THRESHOLD (=90%) of the pixels
marked as field by field_mat are *below*.*/
void BallDetector::CalculateBoundingHorizontalLine(Mat& field_mat, ushort& bounding_horizontal_line)
{
	const double PERCENTAGE_THRESHOLD = 0.995; //Heuristic.
	uint num_of_field_pixels_below_bounding_line=0;
	uint total_num_of_field_pixels=0;
	uint bounding_horizontal_line_ = field_mat.rows; //Initialize the bounding horizontal line to the lowest row of image.
	uint* sum_of_rows_vector=new uint[field_mat.rows]; //Dyanmic array - Will hold the sum of each row in field_mat.

	//Initialize sum_of_rows_vector to zeros:
	for (int i = 0; i < field_mat.rows; i++)
	{
		sum_of_rows_vector[i] = 0;
	}


	//sum all of green points row by row:
	for (uint i = field_mat.rows-1; i > 0; i--)
	{
		for (uint j = 0; j < field_mat.cols; j++)
		{
			sum_of_rows_vector[i] = sum_of_rows_vector[i] + (field_mat.at<uchar>(i, j)^255); //^255(bitwise xor) for conversion from 255=(11111111)_2 to 0 and 0 to 255=(11111111)_2.
		}
		total_num_of_field_pixels = total_num_of_field_pixels + sum_of_rows_vector[i]; //Sum of sum of rows = total num. of field pixels.
	}

	//push the bounding_horizontal_line_ up until  PERCENTAGE_THRESHOLD of the 'field pixels' are below.
	ushort row_num = field_mat.rows - 1;
	num_of_field_pixels_below_bounding_line = sum_of_rows_vector[row_num]; //Initialize to sum of last row of image.
	while (num_of_field_pixels_below_bounding_line / (total_num_of_field_pixels + 0.0) < PERCENTAGE_THRESHOLD)
	{
		row_num--;
		num_of_field_pixels_below_bounding_line = num_of_field_pixels_below_bounding_line + sum_of_rows_vector[row_num]; //add the row above.
	}
	bounding_horizontal_line = row_num; //row_num contains the bounding horizontal line after loop.

	//cout << "bounding horizontal_line:" << bounding_horizontal_line << "\n" <<endl; //TEST

	delete[] sum_of_rows_vector; //Deallocate dyanmic array.
}

double BallDetector::Sigmoid(double x)
{
	return 1/(1+exp(-1*x));
}
