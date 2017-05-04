
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
		SCLK : IN STD_LOGIC;
		MOSI : IN STD_LOGIC;
		SS	  : IN STD_LOGIC;
		Led  : OUT STD_LOGIC_VECTOR (7 downto 0);		--Test led
		MISO : OUT STD_LOGIC
);
end SPI;

architecture Behavioral of SPI is
signal TX	: STD_LOGIC_VECTOR (N-1 DOWNTO 0):= "0000000000000001"; --:= (others => '0'); 
signal RX 	: STD_LOGIC_VECTOR (N-1 DOWNTO 0):= "0000000000000000";     --(others => '0');
signal DataReg		: STD_LOGIC_VECTOR (N-1 downto 0);
signal SCLKShiftReg : STD_LOGIC_VECTOR(1 downto 0) :="00";
signal SSShiftReg : STD_LOGIC_VECTOR(1 downto 0) :="11";


begin

-----------Shift register----------------
ShiftR: process(CLK)

begin 
	if rising_edge(CLK) then
		SCLKShiftReg <= SCLKShiftReg(0) & SCLK;   		--01 rising_edge 10 falling_edge
		SSShiftReg <= SSShiftReg(0) & SS;					--01 rising_edge 10 falling_edge
	end if;
end process;

----------------------------------------

----------Data RX and TX---------------
ReceiveTransmit : process(CLK)
begin
		if rising_edge(CLK) then
			if SSShiftReg = "10" then
				DataReg <= TX;
			end if;
			
			if SSShiftReg = "01" then
				RX <= DataReg;
			end if;
----------------------------------------
		--Write
			if (SCLKShiftReg = "01" and SS = '0') then
			MISO <= DataReg(N-1);
			DataReg(N-1 DOWNTO 1) <= DataReg(N-2 DOWNTO 0); --Shift bit left
		 --Read
			elsif(SCLKShiftReg = "10" and SS = '0') then
			DataReg(0)<= MOSI;
			end if;
		end if;
	end process;

	Led(7 downto 0) <= RX(7 downto 0);


end Behavioral;

