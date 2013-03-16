/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package league.of.yokos;

import java.awt.Color;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.Vector;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextArea;

public class Shop extends JPanel implements ActionListener
{
	public JButton ItemButtons[] = new JButton[64];
	public JButton OwnedItemButtons[] = new JButton[6];
	public JButton ExitButton = new JButton();
	public JButton BuyButton = new JButton();
	public JButton SellButton = new JButton();
	public JLabel ShopTitleLabel = new JLabel();
	
	public JLabel CurrentItemIconLabel = new JLabel();
	public JLabel CurrentItemNameLabel = new JLabel();
	public JLabel CurrentItemPriceLabel = new JLabel();
	public JTextArea CurrentItemEffectsLabel = new JTextArea();
	public Vector<JLabel> CurrentItemPyramid = new Vector();
	
	public boolean SelectedShopItem = false;
	public boolean SelectedInventoryItem = false;
	public int SelectionType = 0;
	public int SelectedSlot = 0;
	
	public static int SELECTED_ITEM_INVENTORY = 1;
	public static int SELECTED_ITEM_SHOP = 2;
	
	public Shop()
	{
		this.setLayout(null);
		this.setLocation(50, 50);
		this.setSize(700, 500);
		this.setBackground(Color.black);
		
		ShopTitleLabel.setForeground(Color.blue);
		ShopTitleLabel.setFont(LeagueOfYokos.ShopTitleFont);
		ShopTitleLabel.setText("League Of Yokos Shop");
		ShopTitleLabel.setLocation(50, 35);
		ShopTitleLabel.setSize(400, 30);
		ShopTitleLabel.setVisible(true);
		this.add(ShopTitleLabel);
		
		ExitButton.setText("X");
		ExitButton.setLocation(this.getWidth() - 50, 5);
		ExitButton.setSize(45, 45);
		ExitButton.setVisible(true);
		ExitButton.addActionListener(this);
		this.add(ExitButton);
		
		BuyButton.setText("Buy");
		BuyButton.setLocation(350, 400);
		BuyButton.setSize(200, 75);
		BuyButton.setVisible(true);
		BuyButton.addActionListener(this);
		this.add(BuyButton);
		
		SellButton.setText("Sell");
		SellButton.setLocation(575, 400);
		SellButton.setSize(100, 75);
		SellButton.setVisible(true);
		SellButton.addActionListener(this);
		this.add(SellButton);
		
		for (int i = 1; i < 63; i++)
		{
			ItemButtons[i] = new JButton();
			//ItemButtons[i].setText(Integer.toString(i));
			ItemButtons[i].setLocation(25 + (((i - 1) / 8) * 40), 75 + (((i - 1) % 8) * 40));
			ItemButtons[i].setSize(25, 25);
			ItemButtons[i].setVisible(true);
			ItemButtons[i].addActionListener(this);
			ItemButtons[i].setIcon(LeagueOfYokos.getItemById(i).icon);
			this.add(ItemButtons[i]);
		}
		
		for (int i = 0; i < 6; i++)
		{
			OwnedItemButtons[i] = new JButton();
			OwnedItemButtons[i].setText(Integer.toString(i));
			OwnedItemButtons[i].setLocation(75 + (i * 30), 410);
			OwnedItemButtons[i].setSize(25, 25);
			OwnedItemButtons[i].setVisible(true);
			OwnedItemButtons[i].addActionListener(this);
			this.add(OwnedItemButtons[i]);
		}
		
		CurrentItemIconLabel.setLocation(420, 75);
		CurrentItemIconLabel.setSize(25, 25);
		this.add(CurrentItemIconLabel);
		CurrentItemNameLabel.setLocation(460, 65);
		CurrentItemNameLabel.setSize(200, 30);
		CurrentItemNameLabel.setForeground(Color.green);
		this.add(CurrentItemNameLabel);
		CurrentItemPriceLabel.setLocation(460, 90);
		CurrentItemPriceLabel.setSize(100, 10);
		CurrentItemPriceLabel.setForeground(Color.yellow);
		this.add(CurrentItemPriceLabel);
		CurrentItemEffectsLabel.setLocation(350, 110);
		CurrentItemEffectsLabel.setSize(200, 100);
		CurrentItemEffectsLabel.setForeground(Color.white);
		CurrentItemEffectsLabel.setEditable(false);
		CurrentItemEffectsLabel.setCursor(null);
		CurrentItemEffectsLabel.setOpaque(false);
		CurrentItemEffectsLabel.setFocusable(false);
		this.add(CurrentItemEffectsLabel);
		
		// You can't buy or sell if nothing is selected.
		BuyButton.setEnabled(false);
		SellButton.setEnabled(false);
	}
	
	public void showItemInformation(int itemID)
	{
		if (itemID == 0)
		{
			CurrentItemIconLabel.setVisible(false);
			CurrentItemNameLabel.setVisible(false);
			CurrentItemPriceLabel.setVisible(false);
			CurrentItemEffectsLabel.setVisible(false);
		}
		else
		{
			Item item = LeagueOfYokos.getItemById(itemID);

			CurrentItemIconLabel.setIcon(item.icon);
			CurrentItemIconLabel.setVisible(true);
			CurrentItemNameLabel.setText(item.Name);
			CurrentItemNameLabel.setVisible(true);
			CurrentItemPriceLabel.setText(Integer.toString(item.getCost()));
			CurrentItemPriceLabel.setVisible(true);
			
			String itemBenefits = "";
			
			if (item.HP != 0) { itemBenefits += "+" + item.HP + " HP\n"; }
			if (item.MP != 0) { itemBenefits += "+" + item.MP + " MP\n"; }
			if (item.HPRegen != 0) { itemBenefits += "+" + item.HPRegen + " HP Regen per 5\n"; }
			if (item.MPRegen != 0) { itemBenefits += "+" + item.MPRegen + " MP Regen per 5\n"; }
			if (item.Armor != 0) { itemBenefits += "+" + item.Armor + " Armor\n"; }
			if (item.MagicResist != 0) { itemBenefits += "+" + item.MagicResist + " Magic Resist\n"; }
			if (item.ArmorPenetrationFlat != 0) { itemBenefits += "+" + item.ArmorPenetrationFlat + " Armor Penetration\n"; }
			if (item.ArmorPenetrationPercent != 0) { itemBenefits += "+" + item.ArmorPenetrationPercent + "% Armor Penetration\n"; }
			if (item.MagicPenetrationFlat != 0) { itemBenefits += "+" + item.MagicPenetrationFlat + " Magic Penetration\n"; }
			if (item.MagicPenetrationPercent != 0) { itemBenefits += "+" + item.MagicPenetrationPercent + "% Magic Penetration\n"; }
			if (item.AttackDamage != 0) { itemBenefits += "+" + item.AttackDamage + " Attack Damage\n"; }
			if (item.AbilityPower != 0) { itemBenefits += "+" + item.AbilityPower + " Ability Power\n"; }
			if (item.AttackSpeedFlat != 0) { itemBenefits += "+" + item.AttackSpeedFlat + " Attack Speed\n"; }
			if (item.AttackSpeedPercent != 0) { itemBenefits += "+" + item.AttackSpeedPercent + "% Attack Speed\n"; }
			if (item.LifeStealPercent != 0) { itemBenefits += "+" + item.LifeStealPercent + "% Lifesteal\n"; }
			if (item.SpellVampPercent != 0) { itemBenefits += "+" + item.SpellVampPercent + "% Spell Vamp\n"; }
			if (item.MovementSpeedFlat != 0) { itemBenefits += "+" + item.MovementSpeedFlat + " Movement Speed\n"; }
			if (item.MovementSpeedPercent != 0) { itemBenefits += "+" + item.MovementSpeedPercent + "% Movement Speed\n"; }
			if (item.CriticalChance != 0) { itemBenefits += "+" + item.CriticalChance + "% Critical Chance\n"; }
			if (item.CooldownReduction != 0) { itemBenefits += "+" + item.CooldownReduction + "% Cooldown Reduction\n"; }
			if (item.Tenacity != 0) { itemBenefits += "+" + item.Tenacity + " Tenacity\n"; }
			if (item.GoldPer10 != 0) { itemBenefits += "+" + item.GoldPer10 + " Gold per 10 seconds\n"; }
			
			CurrentItemEffectsLabel.setText(itemBenefits);
			CurrentItemEffectsLabel.setVisible(true);
			
			if (!CurrentItemPyramid.isEmpty())
			{
				for (int i = 0; i < CurrentItemPyramid.size(); i++)
				{
					this.remove(CurrentItemPyramid.get(i));
				}
				
				CurrentItemPyramid.clear();
			}
			
			Vector<Item> CurrentItems;
			Vector<Item> NextItems;
			int CurrentItemLevel = 0;
			boolean DonePyramid = false;
			
			CurrentItems = new Vector();
			CurrentItems.add(item);
			
			while (!DonePyramid)
			{
				NextItems = new Vector();
				
				for (int curItem = 0; curItem < CurrentItems.size(); curItem++)
				{
					Item thisItem = CurrentItems.get(curItem);
					
					JLabel a = new JLabel();
					a.setSize(25, 25);
					a.setLocation(400 + (curItem * 30), 250 + (CurrentItemLevel * 35));
					a.setVisible(true);
					a.setIcon(thisItem.icon);
					CurrentItemPyramid.add(a);
					this.add(a);
					
					for (int NewItem = 0; NewItem < thisItem.BuildsFrom.size(); NewItem++)
					{
						NextItems.add(LeagueOfYokos.getItemById(thisItem.BuildsFrom.get(NewItem)));
					}
				}
				
				if (NextItems.isEmpty())
				{
					DonePyramid = true;
				}
				else
				{
					CurrentItems = NextItems;
					NextItems = null;
					CurrentItemLevel++;
				}
			}
		}
	}
	
	public void setSelectedItem(int type, int slot)
	{
		if (type == SELECTED_ITEM_INVENTORY)
		{
			SelectedInventoryItem = true;
			SelectedShopItem = false;
			SellButton.setEnabled(true);
			BuyButton.setEnabled(false);
			showItemInformation(LeagueOfYokos.ThisPlayer.getChampion().Items.get(slot).ID);
		}
		else if (type == SELECTED_ITEM_SHOP)
		{
			SelectedInventoryItem = false;
			SelectedShopItem = true;
			SellButton.setEnabled(false);
			BuyButton.setEnabled(true);
			showItemInformation(slot);
		}
		
		SelectedSlot = slot;
	}
	
	@Override
	public void actionPerformed(ActionEvent e)
	{
		if (e.getSource() == ExitButton)
		{
			LeagueOfYokos.frame.remove(this);
		}
		else if (e.getSource() == BuyButton)
		{
			//
		}
		else if (e.getSource() == SellButton)
		{
			//
		}
		else
		{
			for (int i = 0; i < 6; i++)
			{
				if (e.getSource() == OwnedItemButtons[i])
				{
					setSelectedItem(SELECTED_ITEM_INVENTORY, i);
				}
			}

			for (int i = 0; i < 64; i++)
			{
				if (e.getSource() == ItemButtons[i])
				{
					setSelectedItem(SELECTED_ITEM_SHOP, i);
				}
			}
		}
	}
	
}