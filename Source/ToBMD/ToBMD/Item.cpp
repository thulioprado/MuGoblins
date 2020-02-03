#include "Library.h"
#include "Item.h"

CItem::CItem()
{
}

CItem::~CItem()
{
}

void CItem::Convert(const char* From, const char* To, const char* To2)
{
	CMemScript* lpMemScript = new CMemScript;

	if (lpMemScript == 0)
	{
		ErrorMessageBox(MEM_SCRIPT_ALLOC_ERROR, From);
		return;
	}

	if (lpMemScript->SetBuffer(From) == 0)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
		delete lpMemScript;
		return;
	}

	auto Bmd = std::make_unique<BMD[]>(8192);
	auto ExBmd = std::make_unique<ExtraBMD[]>(8192);
	WORD Index;
	int BmdSize = sizeof(BMD[8192]);
	int ExBmdSize = sizeof(ExtraBMD[8192]);

	memset(Bmd.get(), 0, BmdSize);
	memset(ExBmd.get(), 0, ExBmdSize);

	try
	{
		while (true)
		{
			if (lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			int Group = lpMemScript->GetNumber();

			if (Group >= 16)
			{
				ErrorMessageBox("Item group >= 16");
				break;
			}

			while (true)
			{
				if (strcmp("end", lpMemScript->GetAsString()) == 0)
				{
					break;
				}

				Index = GET_ITEM(Group, lpMemScript->GetNumber());

				if (Group >= 8192)
				{
					ErrorMessageBox("Item index >= 8192");
					break;
				}

				Bmd[Index].Slot = lpMemScript->GetAsNumber();
				Bmd[Index].Skill = lpMemScript->GetAsNumber();
				Bmd[Index].Width = lpMemScript->GetAsNumber();
				Bmd[Index].Height = lpMemScript->GetAsNumber();
				Bmd[Index].TwoHand = (Bmd[Index].Width >= 2) ? 1 : 0;
				
				lpMemScript->GetAsNumber();
				lpMemScript->GetAsNumber();
				lpMemScript->GetAsNumber();

				strcpy_s(ExBmd[Index].Name, lpMemScript->GetAsString());
				strncpy_s(Bmd[Index].Name, ExBmd[Index].Name, sizeof(Bmd[Index].Name) - 1);
				
				Bmd[Index].Name[29] = 0;

				if (Group >= 0 && Group <= 5)
				{
					Bmd[Index].Level = lpMemScript->GetAsNumber();
					Bmd[Index].DamageMin = lpMemScript->GetAsNumber();
					Bmd[Index].DamageMax = lpMemScript->GetAsNumber();
					Bmd[Index].AttackSpeed = lpMemScript->GetAsNumber();
					Bmd[Index].Durability = lpMemScript->GetAsNumber();
					Bmd[Index].MagicDurability = lpMemScript->GetAsNumber();
					Bmd[Index].MagicDamageRate = lpMemScript->GetAsNumber();
					Bmd[Index].RequireLevel = lpMemScript->GetAsNumber();
					Bmd[Index].RequireStrength = lpMemScript->GetAsNumber();
					Bmd[Index].RequireDexterity = lpMemScript->GetAsNumber();
					Bmd[Index].RequireEnergy = lpMemScript->GetAsNumber();
					Bmd[Index].RequireVitality = lpMemScript->GetAsNumber();
					Bmd[Index].RequireLeadership = lpMemScript->GetAsNumber();
					Bmd[Index].Durability += Bmd[Index].MagicDurability;
				}
				else if (Group >= 6 && Group <= 11)
				{
					Bmd[Index].Level = lpMemScript->GetAsNumber();
					ExBmd[Index].Defense = lpMemScript->GetAsNumber();
					Bmd[Index].Defense = (BYTE)(ExBmd[Index].Defense);

					if (Group == 6)
					{
						Bmd[Index].DefenseSuccessRate = lpMemScript->GetAsNumber();
					}
					else if (Group >= 7 && Group <= 9)
					{
						Bmd[Index].MagicDefense = lpMemScript->GetAsNumber();
					}
					else if (Group == 10)
					{
						Bmd[Index].AttackSpeed = lpMemScript->GetAsNumber();
					}
					else if (Group == 11)
					{
						Bmd[Index].WalkSpeed = lpMemScript->GetAsNumber();
					}

					Bmd[Index].Durability = lpMemScript->GetAsNumber();
					Bmd[Index].RequireLevel = lpMemScript->GetAsNumber();
					Bmd[Index].RequireStrength = lpMemScript->GetAsNumber();
					Bmd[Index].RequireDexterity = lpMemScript->GetAsNumber();
					Bmd[Index].RequireEnergy = lpMemScript->GetAsNumber();
					Bmd[Index].RequireVitality = lpMemScript->GetAsNumber();
					Bmd[Index].RequireLeadership = lpMemScript->GetAsNumber();
				}
				else if (Group == 12)
				{
					Bmd[Index].Level = lpMemScript->GetAsNumber();
					ExBmd[Index].Defense = lpMemScript->GetAsNumber();
					Bmd[Index].Defense = (BYTE)(ExBmd[Index].Defense);
					Bmd[Index].Durability = lpMemScript->GetAsNumber();
					Bmd[Index].RequireLevel = lpMemScript->GetAsNumber();
					Bmd[Index].RequireEnergy = lpMemScript->GetAsNumber();
					Bmd[Index].RequireStrength = lpMemScript->GetAsNumber();
					Bmd[Index].RequireDexterity = lpMemScript->GetAsNumber();
					Bmd[Index].RequireLeadership = lpMemScript->GetAsNumber();
					Bmd[Index].BuyMoney = lpMemScript->GetAsNumber();
				}
				else if (Group == 13)
				{
					Bmd[Index].Level = lpMemScript->GetAsNumber();
					Bmd[Index].Durability = lpMemScript->GetAsNumber();

					for (int n = 0; n < MAX_RESISTANCE_TYPE; n++)
					{
						Bmd[Index].Resistance[n] = lpMemScript->GetAsNumber();
					}

					Bmd[Index].RequireLevel = Bmd[Index].Level;
				}
				else if (Group == 14)
				{
					Bmd[Index].Value = lpMemScript->GetAsNumber();
					Bmd[Index].Level = lpMemScript->GetAsNumber();
					Bmd[Index].Durability = 1;
				}
				else if (Group == 15)
				{
					Bmd[Index].Level = lpMemScript->GetAsNumber();
					Bmd[Index].RequireLevel = lpMemScript->GetAsNumber();
					Bmd[Index].RequireEnergy = lpMemScript->GetAsNumber();
					Bmd[Index].BuyMoney = lpMemScript->GetAsNumber();
					Bmd[Index].DamageMin = Bmd[Index].Level;
					Bmd[Index].DamageMax = Bmd[Index].Level + (Bmd[Index].Level / 2);
				}

				if (Group <= 11 || Group == 13)
				{
					lpMemScript->GetToken();
				}

				if (Group != 14)
				{
					for (int n = 0; n < MAX_CLASS; n++)
					{
						Bmd[Index].RequireClass[n] = lpMemScript->GetAsNumber();
					}
				}
			}
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;


	for (int i = 0; i < 8192; ++i)
	{
		XorConvert((LPBYTE)(&Bmd[i]), sizeof(BMD));
		XorConvert((LPBYTE)(&ExBmd[i]), sizeof(ExtraBMD));
	}

	FILE* File;

	fopen_s(&File, To, "wb");

	if (File)
	{
		DWORD CRC = 0;

		fwrite(Bmd.get(), BmdSize, 1, File);
		fwrite(&CRC, sizeof(CRC), 1, File);
		fclose(File);
	}

	fopen_s(&File, To2, "wb");

	if (File)
	{
		fwrite(ExBmd.get(), ExBmdSize, 1, File);
		fclose(File);
	}
}

CItem Item;