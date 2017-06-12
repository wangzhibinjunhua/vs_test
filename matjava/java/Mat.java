package com.wzb.helloworld;

import java.util.ArrayList;
/**
 * @author wzb<wangzhibin_x@qq.com>
 * @date Jun 3, 2017 9:02:55 AM	
 */

public class Mat {

	public static void swap(ArrayList<Point> xy, int i, int j) {
		float x=xy.get(i).getX();
		float y=xy.get(i).getY();
		xy.get(i).setX(xy.get(j).getX());
		xy.get(i).setY(xy.get(j).getY());
		xy.get(j).setX(x);
		xy.get(j).setY(y);

	}

	public static float getdist(Point p1, Point p2) {
		return (float) Math.sqrt(
				(p2.getX() - p1.getX()) * (p2.getX() - p1.getX()) + (p2.getY() - p1.getY()) * (p2.getY() - p1.getY()));
	}

	public static float getcross(Point p0, Point p1, Point p2) {
		return (p1.getX() - p0.getX()) * (p2.getY() - p0.getY()) - (p2.getX() - p0.getX()) * (p1.getY() - p0.getY());
	}

	public static float getdot(Point p0, Point p1, Point p2) {
		return (p1.getX() - p0.getX()) * (p2.getX() - p0.getX()) + (p1.getY() - p0.getY()) * (p2.getY() - p0.getY());
	}

	public static float anglecmp(Point p0, Point p1, Point p2) {
		float cross = getcross(p0, p1, p2);
		if (cross == 0) {
			return getdist(p0, p2) - getdist(p0, p1);
		}

		return cross;
	}

	public static void vectorsort(ArrayList<Point> arr, int left, int right) {
		int i, mid, last;

		if (left >= right) {
			return;
		}

		mid = (left + right) / 2;
		swap(arr, left, mid);

		last = left;
		for (i = left + 1; i <= right; i++) {
			if (anglecmp(arr.get(0), arr.get(i), arr.get(left)) > 0) {
				swap(arr, i, ++last);
			}
		}

		swap(arr, left, last);
		vectorsort(arr, left, last - 1);
		vectorsort(arr, last + 1, right);

	}

	public static void getconvex(ArrayList<Point> arr, int len, ArrayList<Count> n) {
		int i, base, top;

		if (len < 4) {
			n.get(0).setN(len);
			return;
		}

		base = 0;
		for (i = 0; i < len; i++) {
			if (arr.get(i).getY() == arr.get(base).getY() && arr.get(i).getX() < arr.get(base).getX()) {
				base = i;
			} else if (arr.get(i).getY() < arr.get(base).getY()) {
				base = i;
			}
		}
		
		System.out.println("base="+base);

		swap(arr, base, 0);
		

		vectorsort(arr, 1, len - 1);
		

		top = 1;
		for (i = 2; i < len; i++) {
			while (top > 0 && getcross(arr.get(top - 1), arr.get(top), arr.get(i)) <= 0) {
				top--;
			}
			
			++top;
			int temp=top;
			arr.get(temp).setX(arr.get(i).getX());
			arr.get(temp).setY(arr.get(i).getY());
		}
		
		n.get(0).setN(top);

	}

	public static void rotatingcalipers(ArrayList<Point> arr, int len, ArrayList<Point> rectangle) {
		int top, down=0, right = 1, up = 0, left = 0, downlast , rightlast, uplast, leftlast;

		float area = Float.MAX_VALUE, dist, X, Y, k;
		Point temp=new Point();
		ArrayList<Count> ilist = new ArrayList<Count>();
		Count c=new Count(0);
		ilist.add(c);
		getconvex(arr, len, ilist);
		top=ilist.get(0).getN();

		++top;
		int ret=top;
		System.out.println("top="+top);
		if(ret==len){
			Point point=new Point();
			arr.add(point);
		}
		arr.get(ret).setX(arr.get(0).getX());
		arr.get(ret).setY(arr.get(0).getY());
		

			
		downlast = down;
		rightlast = right;
		uplast = up;
		leftlast = left;
		for (down = 0; down < top; down++) {
			// find right
			while (getdot(arr.get(down), arr.get(down + 1), arr.get(right)) <= getdot(arr.get(down), arr.get(down + 1),
					arr.get(right + 1))) {
				right = (right + 1) % top;
			}

			// find up
			if (down == 0) {
				up = right;
			}
			while (getcross(arr.get(down), arr.get(down + 1), arr.get(up)) <= getcross(arr.get(down), arr.get(down + 1),
					arr.get(up + 1))) {
				up = (up + 1) % top;
			}

			// find down
			if (down == 0) {
				left = up;
			}

			while (getdot(arr.get(down), arr.get(down + 1), arr.get(left)) >= getdot(arr.get(down), arr.get(down + 1),
					arr.get(left + 1))) {
				left = (left + 1) % top;
			}

			dist = getdist(arr.get(down), arr.get(down + 1));
			X = getcross(arr.get(down), arr.get(down + 1), arr.get(up)) / dist;
			temp.setX(arr.get(right).getX() + arr.get(down).getX() - arr.get(left).getX());
			temp.setY(arr.get(right).getY() + arr.get(down).getY() - arr.get(left).getY());
			Y = getdot(arr.get(down), arr.get(down + 1), temp)/dist;

			if (area > X*Y) {
				area = X*Y;
				downlast = down;
				rightlast = right;
				uplast = up;
				leftlast = left;
			}
		}

		// 计算外接矩形
		if (arr.get(downlast + 1).getY() == arr.get(downlast).getY()) {
			rectangle.get(0).setX(arr.get(leftlast).getX());
			rectangle.get(0).setY(arr.get(downlast).getY());

			rectangle.get(1).setX(arr.get(rightlast).getX());
			rectangle.get(1).setY(arr.get(downlast).getY());

			rectangle.get(2).setX(arr.get(rightlast).getX());
			rectangle.get(2).setY(arr.get(uplast).getY());

			rectangle.get(3).setX(arr.get(leftlast).getX());
			rectangle.get(3).setY(arr.get(uplast).getY());

		}
		else if (arr.get(downlast + 1).getX() == arr.get(downlast).getX()) {
			rectangle.get(0).setX(arr.get(downlast).getX());
			rectangle.get(0).setY(arr.get(leftlast).getY());

			rectangle.get(1).setX(arr.get(downlast).getX());
			rectangle.get(1).setY(arr.get(rightlast).getY());

			rectangle.get(2).setX(arr.get(uplast).getX());
			rectangle.get(2).setY(arr.get(rightlast).getY());

			rectangle.get(3).setX(arr.get(uplast).getX());
			rectangle.get(3).setY(arr.get(leftlast).getY());

		}
		else {
			k = (arr.get(downlast + 1).getY() - arr.get(downlast).getY()) / (arr.get(downlast + 1).getX() - arr.get(downlast).getX());

			rectangle.get(0).setX((float)((k*arr.get(leftlast).getY() + arr.get(leftlast).getX() - k*arr.get(downlast).getY() + k*k*arr.get(downlast).getX()) / (k*k + 1.0)));
			rectangle.get(0).setY((float)(k*rectangle.get(0).getX() + arr.get(downlast).getY() - k*arr.get(downlast).getX()));

			rectangle.get(1).setX((float)((k*arr.get(rightlast).getY() + arr.get(rightlast).getX() - k*arr.get(downlast).getY() + k*k*arr.get(downlast).getX()) / (k*k + 1.0)));
			rectangle.get(1).setY((float)(k*rectangle.get(1).getX() + arr.get(downlast).getY() - k*arr.get(downlast).getX()));

			rectangle.get(2).setX((float)((k*arr.get(rightlast).getY() + arr.get(rightlast).getX() - k*arr.get(uplast).getY() + k*k*arr.get(uplast).getX()) / (k*k + 1.0)));
			rectangle.get(2).setY((float)(k*rectangle.get(2).getX() + arr.get(uplast).getY() - k*arr.get(uplast).getX()));

			rectangle.get(3).setX((float)((k*arr.get(leftlast).getY() + arr.get(leftlast).getX() - k*arr.get(uplast).getY() + k*k*arr.get(uplast).getX()) / (k*k + 1.0)));
			rectangle.get(3).setY((float)(k*rectangle.get(3).getX() + arr.get(uplast).getY() - k*arr.get(uplast).getX()));
		}
	}

	public static void variance_analysis(ArrayList<Point> xy,int len)
	{
		int i;
		float sumx=0.0f,sumy=0.0f,avex=0.0f,avey=0.0f,varx=0.0f,vary=0.0f,stdx=0.0f,stdy=0.0f;
		for (i = 0; i < len; i++) {
			sumx += xy.get(i).getX();
			sumy += xy.get(i).getY();
		}

		//平均值
		avex = sumx / len;
		avey = sumy / len;

		for (i = 0; i < len; i++) {
			varx += (xy.get(i).getX() - avex)*(xy.get(i).getX() - avex);
			vary += (xy.get(i).getY() - avey)*(xy.get(i).getY() - avey);
		}
		//方差
		varx = varx / len;
		vary = vary / len;

		//标准差
		stdx = (float)Math.sqrt(varx);
		stdy = (float)Math.sqrt(vary);

		//printf("stdx=%f,stdy=%f,avex=%f,avey=%f\n", stdx, stdy, avex, avey);

		if (xy.get(0).getX()>(avex + 2 * stdx)) xy.get(0).setX(avex);
		if (xy.get(0).getX() < (avex - 2 * stdx))xy.get(0).setX(avex);

		if (xy.get(0).getY() > (avey + 2 * stdy)) xy.get(0).setY(avey);
		if (xy.get(0).getY() < (avey - 2 * stdy)) xy.get(0).setY(avey);

		for (i = 1; i < len; i++) {
			
			if (xy.get(i).getX()>(avex + 2 * stdx)) xy.get(i).setX(xy.get(i-1).getX());
			if (xy.get(i).getX() < (avex - 2 * stdx)) xy.get(i).setX(xy.get(i-1).getX());

			if (xy.get(i).getY() > (avey + 2 * stdy)) xy.get(i).setY(xy.get(i-1).getY());
			if (xy.get(i).getY() < (avey - 2 * stdy)) xy.get(i).setY(xy.get(i-1).getY());
		}



	}
	
	public static void data_analysis(ArrayList<Point> xy, int len, ArrayList<Point> pxy)
	{
		int i;

		variance_analysis(xy, len);
		ArrayList<Point> b=new ArrayList<Point>(){{add(new Point(1f,1f));add(new Point(1f,1f));add(new Point(1f,1f));add(new Point(1f,1f));}};
	

		ArrayList<Point> xy_temp=new ArrayList<Point>();
		for(i=0;i<len;i++){
			Point point=new Point(xy.get(i).getX(),xy.get(i).getY());
			xy_temp.add(point);
		}
	
		rotatingcalipers(xy_temp, len, b);
		
		for(i=0;i<b.size();i++){
			System.out.println("b:"+b.get(i).getX()+","+b.get(i).getY());
		}

		Point max_y=new Point(0.0f,0.0f);
		Point min_x=new Point(0.0f,0.0f);
		Point min_y=new Point(0.0f,0.0f);
		max_y.setY(b.get(0).getY());
		max_y.setX(b.get(0).getX());
		
		min_x.setY(b.get(0).getY());
		min_x.setX(b.get(0).getX());
	
		min_y.setY(b.get(0).getY());
		min_y.setX(b.get(0).getX());
		

		for (i = 0; i < 4; i++) {
			if (max_y.getY() < b.get(i).getY()) {

				max_y.setY(b.get(i).getY());
				max_y.setX(b.get(i).getX());
			}

			if (min_x.getX() > b.get(i).getX()) {

				min_x.setY(b.get(i).getY());
				min_x.setX(b.get(i).getX());
			}

			if (min_y.getY() >= b.get(i).getY()) {

				min_y.setY(b.get(i).getY());
				min_y.setX(b.get(i).getX());
			}
		}
		
		float sin_yaw,cos_yaw;
		if(min_x.getY()==min_y.getY()){
			sin_yaw=0;
			cos_yaw=1;
		}else{
			sin_yaw = (max_y.getX() - min_x.getX()) / getdist(min_x, max_y);
			cos_yaw = (max_y.getY() - min_x.getY()) / getdist(min_x, max_y);
		}

	
		for (i = 0; i < len; i++) {
			pxy.get(i).setX(xy.get(i).getX()*cos_yaw - xy.get(i).getY()*sin_yaw);
			pxy.get(i).setY(xy.get(i).getY()*cos_yaw + xy.get(i).getX()*sin_yaw);
			

		}

		Point p_max_y=new Point();
		Point p_min_x=new Point();
		Point p_min_y=new Point();
		p_max_y.setX(max_y.getX()*cos_yaw - max_y.getY()*sin_yaw);
		p_max_y.setY(max_y.getY()*cos_yaw + max_y.getX()*sin_yaw);

		p_min_x.setX( min_x.getX()*cos_yaw - min_x.getY()*sin_yaw);
		p_min_x.setY( min_x.getY()*cos_yaw + min_x.getX()*sin_yaw);

		p_min_y.setX(min_y.getX()*cos_yaw - min_y.getY()*sin_yaw);
		p_min_y.setY(min_y.getY()*cos_yaw + min_y.getX()*sin_yaw);

		if ((p_max_y.getY() - p_min_x.getY()) > (p_min_y.getX() - p_min_x.getX())) {
			for (i = 0; i < len; i++) {
				if (p_min_x.getX() < 0)pxy.get(i).setX(pxy.get(i).getX()- p_min_x.getX()+0.000001f);
				if (p_min_x.getY() < 0)pxy.get(i).setY(pxy.get(i).getY()- p_min_x.getY()+0.000001f);
				float temp;
				temp = pxy.get(i).getX();
				pxy.get(i).setX(pxy.get(i).getY());
				pxy.get(i).setY(temp);

				
			}
		}
		else {
			for (i = 0; i < len; i++) {
				if (p_min_x.getX() < 0)pxy.get(i).setX(pxy.get(i).getX()- p_min_x.getX()+0.000001f);
				if (p_min_x.getY() < 0)pxy.get(i).setY(pxy.get(i).getY()- p_min_x.getY()+0.000001f);
			}
		}


	}


}
