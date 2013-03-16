#include "MainForm.h"
#include <Windows.h>
#include "Bot.h"

using namespace DMOBot;

void Main(void)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	Application::Run(gcnew MainForm);
	Application::Exit();
}

void MainForm::AutoChatCheckBox_Click(System::Object^  sender, System::EventArgs^  e)
{
	AutoChatCheckBox->Checked = !AutoChatCheckBox->Checked;
	Bot::bAutoChat = AutoChatCheckBox->Checked;

	if (Bot::bAutoChat)
	{
		Bot::AutoChatDelay = Convert::ToInt32(DMOBot::MainForm::AutoChatDelayTextBox->Text);
		DMOBot::MainForm::AutoChatDelayTextBox->Enabled = false;

		Bot::curAutoChatOffset = 0;
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)&Bot::AutoChatProcedure, 0, 0, 0);
	}
	else
	{
		DMOBot::MainForm::AutoChatDelayTextBox->Enabled = true;
	}
}

void MainForm::AutoPotionCheckBox_Click(System::Object^  sender, System::EventArgs^  e)
{
	AutoPotionCheckBox->Checked = !AutoPotionCheckBox->Checked;
	Bot::TriggerAutoPotion(AutoPotionCheckBox->Checked);
}

void MainForm::FMStealerCheckBox_Click(System::Object^  sender, System::EventArgs^  e)
{
	FMStealerCheckBox->Checked = !FMStealerCheckBox->Checked;
	Bot::TriggerFMStealer(FMStealerCheckBox->Checked);
}

void MainForm::AutoChatLoadButton_Click(System::Object^  sender, System::EventArgs^  e)
{
	File autoChatFile;
	autoChatFile.Path = "autochat.txt";
	autoChatFile.Open();
	Bot::autoChatStr = autoChatFile.readAllLines();
	autoChatFile.Close();
	MessageBox::Show(Bot::autoChatStr.CountLines() + " chat lines loaded from \"autochat.txt\".");
}

void MainForm::AutoPotSetHPButton_Click(System::Object^  sender, System::EventArgs^  e)
{
	GetCursorPos(&Bot::HPpotPos);
	Bot::HPpotPos.x -= 2;
	Bot::HPpotPos.y -= 2;
	Bot::UpdateScreenCapture();
	Bot::HPpotClr.Set(Bot::screenCap.Get(Bot::HPpotPos.x, Bot::HPpotPos.y));
}

void MainForm::AutoPotSetMPButton_Click(System::Object^  sender, System::EventArgs^  e)
{
	GetCursorPos(&Bot::MPpotPos);
	Bot::MPpotPos.x -= 2;
	Bot::MPpotPos.y -= 2;
	Bot::UpdateScreenCapture();
	Bot::MPpotClr.Set(Bot::screenCap.Get(Bot::MPpotPos.x, Bot::MPpotPos.y));
}

void MainForm::FMStealerSetButton_Click(System::Object^  sender, System::EventArgs^  e)
{
	GetCursorPos(&Bot::fmPos);
	Bot::fmPos.x -= 20;
	Bot::fmPos.y -= 20;
	Bot::UpdateScreenCapture();
	Bot::fmClr.Set(Bot::screenCap.Get(Bot::fmPos.x, Bot::fmPos.y));
	/*
		POINT p;
		GetCursorPos(&p);
		char f[10];
		char final[20];
		ZeroMemory(final, sizeof(final));
		_itoa(p.x, f, 10);
		strcat(final, f);
		strcat(final, ", ");
		_itoa(p.y, f, 10);
		strcat(final, f);
		MessageBoxA(0, final, final, MB_OK);
		*/
}