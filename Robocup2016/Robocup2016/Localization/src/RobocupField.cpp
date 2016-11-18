#include "../headers/RobocupField.h"

PointOfInterest RobocupField::CirclePOIs[CPOICount] = { PointOfInterest(0,0,PointOfInterest::CIRCLE_SHAPE,"CENTER_FIELD") };

PointOfInterest RobocupField::TTypePOIs[TPOICount] = { PointOfInterest(0,B * 0.5f,PointOfInterest::T_SHAPE,"CENTER_TOP_T"),
													   PointOfInterest(0,-B * 0.5f,PointOfInterest::T_SHAPE,"CENTER_LOW_T"),
													   PointOfInterest(A * 0.5f, D * 0.5f,PointOfInterest::T_SHAPE,"RIGHT_TOP_POST_T"),
													   PointOfInterest(A * 0.5f, F * 0.5f,PointOfInterest::T_SHAPE,"RIGHT_TOP_GOALIE_AREA_T"),
													   PointOfInterest(A * 0.5f, -D * 0.5f,PointOfInterest::T_SHAPE,"RIGHT_LOW_POST_T"),
													   PointOfInterest(A * 0.5f, -F * 0.5f,PointOfInterest::T_SHAPE,"RIGHT_LOW_GOALIE_AREA_T"),
													   PointOfInterest(-A * 0.5f, D * 0.5f,PointOfInterest::T_SHAPE,"LEFT_TOP_POST_T"),
													   PointOfInterest(-A * 0.5f, F * 0.5f,PointOfInterest::T_SHAPE,"LEFT_TOP_GOALIE_AREA_T"),
														PointOfInterest(-A * 0.5f, -D * 0.5f,PointOfInterest::T_SHAPE,"LEFT_LOW_POST_T"),
														PointOfInterest(-A * 0.5f, -F * 0.5f,PointOfInterest::T_SHAPE,"LEFT_LOW_GOALIE_AREA_T")};


PointOfInterest RobocupField::LTypePOIs[LPOICount] = { PointOfInterest(A * 0.5f, B * 0.5f,PointOfInterest::L_SHAPE,"RIGHT_TOP_FIELD_CORNER_L"),
													   PointOfInterest(A * 0.5f + C, D * 0.5f,PointOfInterest::L_SHAPE,"RIGHT_TOP_BACK_NET_L"),
													   PointOfInterest(A * 0.5f - E, F * 0.5f,PointOfInterest::L_SHAPE,"RIGHT_TOP_FRONT_GOAL_AREA_L"),
													   PointOfInterest(A * 0.5f, -B * 0.5f,PointOfInterest::L_SHAPE,"RIGHT_LOW_FIELD_CORNER_L"),
													   PointOfInterest(A * 0.5f + C,-D * 0.5f,PointOfInterest::L_SHAPE,"RIGHT_LOW_BACK_NET_L"),
													   PointOfInterest(A * 0.5f - E, -F * 0.5f,PointOfInterest::L_SHAPE,"RIGHT_LOW_FRONT_GOAL_AREA_L"),
													   PointOfInterest(-A * 0.5f, B * 0.5f,PointOfInterest::L_SHAPE,"LEFT_TOP_FIELD_CORNER_L"),
													   PointOfInterest(-A * 0.5f + C, D * 0.5f,PointOfInterest::L_SHAPE,"LEFT_TOP_BACK_NET_L"),
													   PointOfInterest(-A * 0.5f - E, F * 0.5f,PointOfInterest::L_SHAPE,"LEFT_TOP_FRONT_GOAL_AREA_L"),
													   PointOfInterest(-A * 0.5f, -B * 0.5f,PointOfInterest::L_SHAPE,"LEFT_LOW_FIELD_CORNER_L"),
													   PointOfInterest(-A * 0.5f + C,-D * 0.5f,PointOfInterest::L_SHAPE,"LEFT_LOW_BACK_NET_L"),
													   PointOfInterest(-A * 0.5f - E, -F * 0.5f,PointOfInterest::L_SHAPE,"LEFT_LOW_FRONT_GOAL_AREA_L") };

	
PointOfInterest RobocupField::DotPOIs[DPOICount] = { PointOfInterest(A * 0.5f - G ,0,PointOfInterest::DOT_SHAPE,"RIGHT_PENELTY_DOT"),
													 PointOfInterest(-A * 0.5f + G ,0,PointOfInterest::DOT_SHAPE,"LEFT_PENELTY_DOT") };


PointOfInterest RobocupField::PlusPOIs[PPOICount] = { PointOfInterest(0, H*0.5f,PointOfInterest::PLUS_SHAPE,"CENTER_TOP_PLUS"),
													  PointOfInterest(0, -H*0.5f,PointOfInterest::DOT_SHAPE,"CENTER_LOW_PLUS") };




PointOfInterest RobocupField::AllPOIs[AllCount] = { PointOfInterest(0,0,PointOfInterest::CIRCLE_SHAPE,"CENTER_FIELD"),
													PointOfInterest(0,B * 0.5f,PointOfInterest::T_SHAPE,"CENTER_TOP_T"),
													PointOfInterest(0,-B * 0.5f,PointOfInterest::T_SHAPE,"CENTER_LOW_T"),
													PointOfInterest(A * 0.5f, D * 0.5f,PointOfInterest::T_SHAPE,"RIGHT_TOP_POST_T"),
													PointOfInterest(A * 0.5f, F * 0.5f,PointOfInterest::T_SHAPE,"RIGHT_TOP_GOALIE_AREA_T"),
													PointOfInterest(A * 0.5f, -D * 0.5f,PointOfInterest::T_SHAPE,"RIGHT_LOW_POST_T"),
													PointOfInterest(A * 0.5f, -F * 0.5f,PointOfInterest::T_SHAPE,"RIGHT_LOW_GOALIE_AREA_T"),
													PointOfInterest(-A * 0.5f, D * 0.5f,PointOfInterest::T_SHAPE,"LEFT_TOP_POST_T"),
													PointOfInterest(-A * 0.5f, F * 0.5f,PointOfInterest::T_SHAPE,"LEFT_TOP_GOALIE_AREA_T"),
													PointOfInterest(-A * 0.5f, -D * 0.5f,PointOfInterest::T_SHAPE,"LEFT_LOW_POST_T"),
													PointOfInterest(-A * 0.5f, -F * 0.5f,PointOfInterest::T_SHAPE,"LEFT_LOW_GOALIE_AREA_T"),
													PointOfInterest(A * 0.5f, B * 0.5f,PointOfInterest::L_SHAPE,"RIGHT_TOP_FIELD_CORNER_L"),
													PointOfInterest(A * 0.5f + C, D * 0.5f,PointOfInterest::L_SHAPE,"RIGHT_TOP_BACK_NET_L"),
													PointOfInterest(A * 0.5f - E, F * 0.5f,PointOfInterest::L_SHAPE,"RIGHT_TOP_FRONT_GOAL_AREA_L"),
													PointOfInterest(A * 0.5f, -B * 0.5f,PointOfInterest::L_SHAPE,"RIGHT_LOW_FIELD_CORNER_L"),
													PointOfInterest(A * 0.5f + C,-D * 0.5f,PointOfInterest::L_SHAPE,"RIGHT_LOW_BACK_NET_L"),
													PointOfInterest(A * 0.5f - E, -F * 0.5f,PointOfInterest::L_SHAPE,"RIGHT_LOW_FRONT_GOAL_AREA_L"),
													PointOfInterest(-A * 0.5f, B * 0.5f,PointOfInterest::L_SHAPE,"LEFT_TOP_FIELD_CORNER_L"),
													PointOfInterest(-A * 0.5f + C, D * 0.5f,PointOfInterest::L_SHAPE,"LEFT_TOP_BACK_NET_L"),
													PointOfInterest(-A * 0.5f - E, F * 0.5f,PointOfInterest::L_SHAPE,"LEFT_TOP_FRONT_GOAL_AREA_L"),
													PointOfInterest(-A * 0.5f, -B * 0.5f,PointOfInterest::L_SHAPE,"LEFT_LOW_FIELD_CORNER_L"),
													PointOfInterest(-A * 0.5f + C,-D * 0.5f,PointOfInterest::L_SHAPE,"LEFT_LOW_BACK_NET_L"),
													PointOfInterest(-A * 0.5f - E, -F * 0.5f,PointOfInterest::L_SHAPE,"LEFT_LOW_FRONT_GOAL_AREA_L"),
													PointOfInterest(A * 0.5f - G ,0,PointOfInterest::DOT_SHAPE,"RIGHT_PENELTY_DOT"),
													PointOfInterest(-A * 0.5f + G ,0,PointOfInterest::DOT_SHAPE,"LEFT_PENELTY_DOT"),
													PointOfInterest(0, H*0.5f,PointOfInterest::PLUS_SHAPE,"CENTER_TOP_PLUS"),
													PointOfInterest(0, -H*0.5f,PointOfInterest::DOT_SHAPE,"CENTER_LOW_PLUS") };








