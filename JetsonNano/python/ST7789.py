import config
import RPi.GPIO as GPIO
import time
import numpy as np


class ST7789(object):
    def __init__(self):
		self.width = 160
		self.height = 160
		#Initialize DC RST pin
		self._dc = config.DC_PIN
		self._rst = config.RST_PIN
		self._bl = config.BL_PIN



    """    Write register address and data     """
    def command(self, cmd):
        GPIO.output(self._dc, GPIO.LOW)
        config.spi_writebyte([cmd])

    def data(self, val):
        GPIO.output(self._dc, GPIO.HIGH)
        config.spi_writebyte([val])

    def Init(self):
        if (config.module_init() != 0):
            return -1
        """Initialize dispaly"""    
        self.reset()

        self.command(0x11);
        time.sleep(0.1)
        self.command(0x21); 
        self.command(0x21); 

        self.command(0xB1); 
        self.data(0x05);
        self.data(0x3A);
        self.data(0x3A);

        self.command(0xB2);
        self.data(0x05);
        self.data(0x3A);
        self.data(0x3A);

        self.command(0xB3); 
        self.data(0x05);  
        self.data(0x3A);
        self.data(0x3A);
        self.data(0x05);
        self.data(0x3A);
        self.data(0x3A);

        self.command(0xB4);
        self.data(0x03);

        self.command(0xC0);
        self.data(0x62);
        self.data(0x02);
        self.data(0x04);

        self.command(0xC1);
        self.data(0xC0);

        self.command(0xC2);
        self.data(0x0D);
        self.data(0x00);

        self.command(0xC3);
        self.data(0x8D);
        self.data(0x6A);   

        self.command(0xC4);
        self.data(0x8D); 
        self.data(0xEE); 

        self.command(0xC5);
        self.data(0x0E);    

        self.command(0xE0);
        self.data(0x10);
        self.data(0x0E);
        self.data(0x02);
        self.data(0x03);
        self.data(0x0E);
        self.data(0x07);
        self.data(0x02);
        self.data(0x07);
        self.data(0x0A);
        self.data(0x12);
        self.data(0x27);
        self.data(0x37);
        self.data(0x00);
        self.data(0x0D);
        self.data(0x0E);
        self.data(0x10);

        self.command(0xE1);
        self.data(0x10);
        self.data(0x0E);
        self.data(0x03);
        self.data(0x03);
        self.data(0x0F);
        self.data(0x06);
        self.data(0x02);
        self.data(0x08);
        self.data(0x0A);
        self.data(0x13);
        self.data(0x26);
        self.data(0x36);
        self.data(0x00);
        self.data(0x0D);
        self.data(0x0E);
        self.data(0x10);

        self.command(0x3A); 
        self.data(0x05);

        self.command(0x36);
        self.data(0xA8);

        self.command(0x29); 

    def reset(self):
        """Reset the display"""
        GPIO.output(self._rst,GPIO.HIGH)
        time.sleep(0.1)
        GPIO.output(self._rst,GPIO.LOW)
        time.sleep(0.1)
        GPIO.output(self._rst,GPIO.HIGH)
        time.sleep(0.1)
        
    def SetWindows(self, Xstart, Ystart, Xend, Yend):
        #set the X coordinates
		Xstart=Xstart+1;
		Xend=Xend+1;
		Ystart=Ystart+26;
		Yend=Yend+26;
		self.command(0x2A)
		self.data(0x00)               #Set the horizontal starting point to the high octet
		self.data(Xstart & 0xff)      #Set the horizontal starting point to the low octet
		self.data(0x00)               #Set the horizontal end to the high octet
		self.data((Xend - 1) & 0xff) #Set the horizontal end to the low octet 

		#set the Y coordinates
		self.command(0x2B)
		self.data(0x00)
		self.data((Ystart & 0xff))
		self.data(0x00)
		self.data((Yend - 1) & 0xff )

		self.command(0x2C)    
    
    def ShowImage(self,Image,Xstart,Ystart):
        """Set buffer to value of Python Imaging Library image."""
        """Write display buffer to physical display"""
        imwidth, imheight = Image.size
        if imwidth != self.width or imheight != self.height:
            raise ValueError('Image must be same dimensions as display \
                ({0}x{1}).' .format(self.width, self.height))
			
        img = np.asarray(Image)
		
        pix = np.zeros((self.width,self.height,2), dtype = np.uint8)
        pix[...,[0]] = np.add(np.bitwise_and(img[...,[0]],0xF8), np.right_shift(img[...,[1]],5))
        pix[...,[1]] = np.add(np.bitwise_and(np.left_shift(img[...,[1]],3),0xE0), np.right_shift(img[...,[2]],3))
		
        pix = pix.flatten().tolist()  	
        self.SetWindows ( 0, 0, self.width, self.height)
        GPIO.output(self._dc,GPIO.HIGH)

        for i in range(0,len(pix)/2,1):#The length is 160*160*2 Actually 160*80*2 So take 2
			config.spi_writebyte(pix[i:i+1])
	
        
    def clear(self):
        """Clear contents of image buffer"""
        _buffer = [0xff]*(self.width * self.height )
        self.SetWindows ( 0, 0, self.width, self.height)
        GPIO.output(self._dc,GPIO.HIGH)
        for i in range(0,len(_buffer),1):
            config.spi_writebyte(_buffer[i:i+1])		
            #print "%d",_buffer[i:i+4096]
