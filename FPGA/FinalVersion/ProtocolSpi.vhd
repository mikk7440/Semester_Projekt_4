
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;


entity ProtocolSpi is
generic (
		N    : INTEGER := 16
);
Port(
		CLK			: in STD_LOGIC;
		Encoder1 	: in STD_LOGIC_VECTOR(11 downto 0);
		Encoder2 	: in STD_LOGIC_VECTOR(11 downto 0);
		Velocity1	: in STD_LOGIC_VECTOR(11 downto 0);
		Velocity2	: in STD_LOGIC_VECTOR(11 downto 0);
		Current1		: in	STD_LOGIC_VECTOR(11 downto 0);
		Current2		: in STD_LOGIC_VECTOR(11 downto 0);
		Motor1Duty 	: out STD_LOGIC_VECTOR(7 downto 0);
		Motor2Duty 	: out STD_LOGIC_VECTOR(7 downto 0);
		protocolbits: out STD_LOGIC_VECTOR(2 downto 0);
		SSCLK 			: IN STD_LOGIC;
		MOSI 			: IN STD_LOGIC;
		SS	  			: IN STD_LOGIC;
		MISO 			: OUT STD_LOGIC;
--		Led  : OUT STD_LOGIC_VECTOR(7 downto 0);
--		ResetPWM1	: OUT STD_LOGIC;
--		ResetPWM2	: OUT STD_LOGIC;
		HomePos 		: OUT STD_LOGIC

);
end ProtocolSpi;

architecture Behavioral of ProtocolSpi is
component SPI
    port(
		CLK  : in STD_LOGIC;
		SSCLK : IN STD_LOGIC;
		MOSI : IN STD_LOGIC;
		SS	  : IN STD_LOGIC;
		TX	  : IN STD_LOGIC_VECTOR(N-1 downto 0);
		RX	  : OUT STD_LOGIC_VECTOR(N-1 downto 0);
		MISO : OUT STD_LOGIC;
		ProtocolBit : OUT STD_LOGIC_VECTOR(2 downto 0);
		busyFlag : out STD_LOGIC
		);
end component;
signal DataTX : STD_LOGIC_VECTOR(N-1 downto 0);
signal DataRX : STD_LOGIC_VECTOR(N-1 downto 0):= (others =>'0');
signal TempReset : STD_LOGIC;
SIGNAL ProtocolBit : STD_LOGIC_VECTOR(2 DOWNTO 0);
signal busyFlag : STD_LOGIC;
begin
U11 : SPI PORT MAP(CLK=>CLK,
						SSCLK=>SSCLK,
						MOSI=>MOSI,
						SS=>SS,
						MISO=>MISO,
						TX=>DataTX,
						RX=>DataRX,
						ProtocolBit=>ProtocolBit,
						busyFlag =>busyFlag
						);
						
HomePos <= TempReset;
protocolbits <= ProtocolBit;



Protokol : process(CLk)
variable DummyToggle : STD_LOGIC :='0';
variable TempRX 	 : STD_LOGIC_VECTOR(2 downto 0);
begin

	if rising_edge(CLK) then
		TempReset <= '0';
															--TempRX := DataRX(N-1 downto N-4); ------------(others => '0');
-----------Pulse Protocol bits--------------------	
		if "111" /= ProtocolBit or "000" /= ProtocolBit then 
			TempRX := ProtocolBit;
		end if;
---------------------------------------------------
		
		case	TempRX is
			when "111" => 
								TempReset <= '1'; -------Test
								if DummyToggle = '0' then
									TempReset <= '1';
									TempRX := "000";
									DummyToggle := '1';
								end if;


			when "001" => 
								--if busyFlag = '0' then
									Motor1Duty <= DataRX(7 downto 0);
									DataTX <= (others => '0');--"00" & Current1; --Encoder1; --MODTAG STRØM
									TempRX := "000";
								--end if;
--			when "0010" =>DataTX <= "0000" & Current1;
--							TempRX := "0000";
--			when "0011" =>DataTX <= "0000" & Velocity1;
--								TempRX := "0000";
			when "010" =>DataTX <= "0000" & Encoder1;
							TempRX := "000";


			when "011" => 
								--if busyFlag = '0' then
									Motor2Duty <= DataRX(7 downto 0);
									DataTX <= (others => '0');--"10" & Encoder2;   ---mODTAG STRØM
									TempRX := "000";
								--end if;
--			when "0110" =>DataTX <= "1000" & Current2;
--								TempRX := "0000";
--			when "0111" =>DataTX <= "1000" & Velocity2;
--							TempRX := "0000";
			when "100" =>DataTX <= "0001" & Encoder2;
							TempRX := "000";
			
			
	--		when "1101" => DataTX <= "000000000000" & ProtocolBit;-- test
	
			when others => DataTX <= (others =>'0');
								TempRX := "000";
		end case;
--		Led(7 downto 4) <= TempRX;
--		Led(3 downto 0) <= DataRX(N-1 downto N-4);
	end if;
end process;


end Behavioral;

