
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity SPI is
generic (
		N    : INTEGER := 16
);
port( 
		CLK  : in STD_LOGIC;
		SSCLK : IN STD_LOGIC;
		MOSI : IN STD_LOGIC;
		SS	  : IN STD_LOGIC;
		TX	  : IN STD_LOGIC_VECTOR(N-1 downto 0);
		RX	  : OUT STD_LOGIC_VECTOR(N-1 downto 0);
		MISO : OUT STD_LOGIC;
		ProtocolBit : OUT STD_LOGIC_VECTOR(2 downto 0);
		busyFlag	: out STD_LOGIC
		
);
end SPI;

architecture Behavioral of SPI is
--signal TX	: STD_LOGIC_VECTOR (N-1 DOWNTO 0):= (others => '0'); 
--signal RX 	: STD_LOGIC_VECTOR (N-1 DOWNTO 0):= (others => '0');
signal DataReg		: STD_LOGIC_VECTOR (N-1 downto 0);
signal SCLKShiftReg : STD_LOGIC_VECTOR(1 downto 0) :="00";
signal SSShiftReg : STD_LOGIC_VECTOR(1 downto 0) :="11";		--Active Low

begin

-----------Shift register----------------
ShiftR: process(CLK)

begin 
	if rising_edge(CLK) then
		SCLKShiftReg <= SCLKShiftReg(0) & SSCLK;   		--01 rising_edge 10 falling_edge
		SSShiftReg <= SSShiftReg(0) & SS;					--01 rising_edge 10 falling_edge
	end if;
end process;

----------------------------------------

----------Data RX and TX---------------
ReceiveTransmit : process(CLK)
variable Counter : STD_LOGIC_VECTOR(4 downto 0) := (others => '0');
begin
		if rising_edge(CLK) then
			if SSShiftReg = "10" then
				DataReg <= TX;
				busyFlag <= '1';
			end if;
			
			if SSShiftReg = "01" then
				RX <= DataReg;
				busyFlag <= '0';
			end if;
----------------------------------------
		--Write
			if (SCLKShiftReg = "01" and SS = '0') then
				MISO <= DataReg(N-1);
				DataReg(N-1 DOWNTO 1) <= DataReg(N-2 DOWNTO 0); --Shift bit left
		 --Read
			elsif(SCLKShiftReg = "10" and SS = '0') then
				DataReg(0)<= MOSI;
				Counter := Counter +1;
			end if;
		 --Getting protocol bit
			if SCLKShiftReg = "00" then
				if Counter = 3 then
					ProtocolBit <= DataReg(2 downto 0);
					DataReg(15 downto 3) <= TX(12 downto 0);
				end if;
			end if;
			--------Set to try state----- 
			if SS = '1' then
				MISO <= 'Z';
				Counter :=(others => '0');
			end if;
			--------End-----------------
		end if;

	
	end process;

end Behavioral;
