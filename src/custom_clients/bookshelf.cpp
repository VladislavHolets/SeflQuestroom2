/*
 * bookshelf.cpp
 *
 *  Created on: Feb 16, 2022
 *      Author: piatk
 */

#include <custom_clients/bookshelf.h>
#include <peripherals/multpinsextender.h>
#include <peripherals/pwmpinsextender.h>
#include <USBSerial.h>
#include <wiring_constants.h>
#include <wiring_digital.h>
#include <wiring_time.h>
#include <WSerial.h>

namespace SEFL
{

	Bookshelf::Bookshelf(MQTTClient &mqtt, uint8_t servo_pins[], uint8_t sensor_pins[],
						 uint8_t book_magnet_pin, const char *name, uint8_t reset_status,
						 const char *placement, const char *in_topic, const char *out_topic,
						 SEFL::Language language) : Quest_Basic_Client(mqtt, name, reset_status, placement, in_topic,
																	   out_topic, language),
													book_iterator(0), number(0)
	{
		// TODO Auto-generated constructor stub
		for (uint8_t i = 0; i < 4; i++)
		{
			this->book_sensor_pins_[i] = sensor_pins[i];
			this->book_servo_pins_[i] = servo_pins[i];
		}
		this->book_magnet_pin_ = book_magnet_pin;
		Pext.digitalWrite(book_magnet_pin_, LOW);
		Pext.getHandler()->setPWMFreqServo();
	}

	Bookshelf::~Bookshelf()
	{
		// TODO Auto-generated destructor stub
	}

	void Bookshelf::onActive()
	{
		if (this->isChangedStatus())
		{
			number = 0;
			this->reportStatus();
			this->unsetChangedStatus();
			Pext.getHandler()->setPWMFrequency(50);
			digitalWrite(this->book_magnet_pin_, LOW);
			for (uint8_t i = 0; i < 4; i++)
			{
				pinMode(this->book_sensor_pins_[i], INPUT_PULLUP);
				Pext.getHandler()->setChannelPWM(order[i],
												 this->books_[i].pwmForAngle(180)); // ������ ����������� �� ������� �������� ���������� 'pos'
																					// delay(2);                    // ��� 15ms ��� ���������� �������
																					//		}
			}
			delay(700);
			for (uint8_t i = 0; i < 4; i++)
			{

				//			for (int pos = this->books_[i].read(); pos >= 70; pos -= 1) { // �� 0 �� 180 ��������
				//				// ��� � ���� ������
				Pext.getHandler()->setChannelPWM(order[i],
												 this->books_[i].pwmForAngle(70)); // ������ ����������� �� ������� �������� ���������� 'pos'
			}
			// delay(2);                    // ��� 15ms ��� ���������� �������
			//			}
			delay(500);

			this->book_iterator = 0;
			for (int i = 0; i < 4; i++)
			{
				numbers[i] = 0;

				Pext.getHandler()->setChannelPWM(order[i], 0); // ������ ����������� �� ������� �������� ���������� 'pos'
			}
		}

		bool statuses[4];
		Serial.print("numbers ");
		for (int i = 0; i < 4; i++)
		{
			statuses[i] = Mext.digitalRead(this->book_sensor_pins_[i]);
			Serial.print(" : ");
			// Serial.print(statuses[i]);
			if (!statuses[i] && numbers[i] == 0)
			{
				numbers[i] = ++number;
			}
			else if (statuses[i] && numbers[i])
			{
				numbers[i] = 0;
			}

			Serial.print(numbers[i]);
		}
		book_iterator = 0;
		for (int i = 0; i < 4; i++)
		{
			if (numbers[i])
			{
				book_iterator++;
			}
			else
			{
				book_iterator = 0;
			}
		}
		if (book_iterator >= 4)
		{

			bool result = 1;
			for (int i = 0; i < 3; i++)
			{
				if (!(numbers[i] < numbers[i + 1]))
				{
					result = 0;
				}
			}

			if (result == 1)
			{
				this->setStatus(SEFL::BasicClientStatuses::FINISHED_STATUS);

				return;
			}
		}

		Serial.print("\ncounter: ");
		Serial.println(book_iterator);
		int reset = 0;

		if (book_iterator >= 4)
		{
			book_iterator = 0;

			uint32_t timestamps[4];
			for (uint8_t i = 0; i < 4; i++)
			{
				Pext.getHandler()->setChannelPWM(order[i],
												 this->books_[i].pwmForAngle(180)); // ������ ����������� �� ������� �������� ���������� 'pos'
				// delay(2);
				delay(300); // ��� 15ms ��� ���������� �������
			}
			delay(200);

			for (uint8_t i = 0; i < 4; i++)
			{ // ��� � ���� ������
				Pext.getHandler()->setChannelPWM(order[i],
												 this->books_[i].pwmForAngle(70)); // ������ ����������� �� ������� �������� ���������� 'pos'
				// delay(2);

				delay(300); // ��� 15ms ��� ���������� �������
			}
			delay(200);
			for (uint8_t i = 0; i < 4; i++)
			{
				Pext.getHandler()->setChannelPWM(order[i], 0);
			}
		}
	}

	void Bookshelf::onFinished()
	{
		if (this->isChangedStatus())
		{
			this->reportStatus();
			this->unsetChangedStatus();
			pinMode(PA1, OUTPUT);
			digitalWrite(PA1, LOW);
			//		for (uint8_t i = 0; i < 4; i++) {
			//			for (int pos = this->books_[i].read(); pos <= 180; pos += 1) { // �� 0 �� 180 ��������
			//				// ��� � ���� ������
			//				this->books_[i].write(pos); // ������ ����������� �� ������� �������� ���������� 'pos'
			//				delay(2);                    // ��� 15ms ��� ���������� �������
			//			}
			//			for (int pos = this->books_[i].read(); pos >= 90; pos -= 1) { // �� 0 �� 180 ��������
			//				// ��� � ���� ������
			//				this->books_[i].write(pos); // ������ ����������� �� ������� �������� ���������� 'pos'
			//				delay(2);                    // ��� 15ms ��� ���������� �������
			//			}
			//		}

			Pext.digitalWrite(this->book_magnet_pin_, HIGH);
		}
	}

	void Bookshelf::onManualFinished()
	{
		if (this->isChangedStatus())
		{
			this->reportStatus();
			this->unsetChangedStatus();
			for (uint8_t i = 0; i < 4; i++)
			{

				Pext.getHandler()->setChannelPWM(order[i],
												 this->books_[i].pwmForAngle(0)); // ������ ����������� �� ������� �������� ���������� 'pos'
				delay(700);
				Pext.getHandler()->setChannelPWM(order[i],
												 0); // ������ ����������� �� ������� �������� ���������� 'pos'
			}
			Pext.digitalWrite(this->book_magnet_pin_, HIGH);
			pinMode(PA1, OUTPUT);
			digitalWrite(PA1, LOW);
		}
	}

	void Bookshelf::onDefault()
	{
		if (this->isChangedStatus())
		{
			this->reportStatus();
			this->unsetChangedStatus();

			digitalWrite(this->book_magnet_pin_, LOW);
			for (uint8_t i = 0; i < 4; i++)
			{

				Pext.getHandler()->setChannelPWM(order[i],
												 this->books_[i].pwmForAngle(0)); // ������ ����������� �� ������� �������� ���������� 'pos'
																				  // delay();                // ��� 15ms ��� ���������� �������
			}
			delay(700);

			for (uint8_t i = 0; i < 4; i++)
			{
				Pext.getHandler()->setChannelPWM(order[i],
												 0); // ������ ����������� �� ������� �������� ���������� 'pos'
			}
		}
		Pext.digitalWrite(this->book_magnet_pin_, LOW);
		pinMode(PA1, OUTPUT);
		digitalWrite(PA1, LOW);
	}

} /* namespace SEFL */
