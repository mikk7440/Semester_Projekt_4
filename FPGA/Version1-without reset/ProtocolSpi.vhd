
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;


entity ProtocolSpi is
generic (
		N    : INTEGER := 16
);
Port(
		CLK	: in STD_LOGIC;
		Encoder1 : in STD_LOGIC_VECTOR(13 downto 0);
		Encoder2 : in STD_LOGIC_VECTOR(13 downto 0);
		Motor1Duty : out STD_LOGIC_VECTOR(7 downto 0);
		Motor2Duty : out STD_LOGIC_VECTOR(7 downto 0);	
		SCLK : IN STD_LOGIC;
		MOSI : IN STD_LOGIC;
		SS	  : IN STD_LOGIC;
		MISO : OUT STD_LOGIC;
		Led  : OUT STD_LOGIC_VECTOR(7 downto 0);
		ResetPWM1	: OUT STD_LOGIC;
		ResetPWM2	: OUT STD_LOGIC;
		ResetEncoder : OUT STD_LOGIC

);
end ProtocolSpi;

architecture Behavioral of ProtocolSpi is
component SPI
    port(
		CLK  : in STD_LOGIC;
		SCLK : IN STD_LOGIC;
		MOSI : IN STD_LOGIC;
		SS	  : IN STD_LOGIC;
		TX	  : IN STD_LOGIC_VECTOR(N-1 downto 0);
		RX	  : OUT STD_LOGIC_VECTOR(N-1 downto 0);
		MISO : OUT STD_LOGIC
		);
end component;
signal DataTX : STD_LOGIC_VECTOR(N-1 downto 0);
signal DataRX : STD_LOGIC_VECTOR(N-1 downto 0);
signal TempReset : STD_LOGIC;
signal PastDataRX :STD_LOGIC_VECTOR(N-1 downto 0);

begin
U11 : SPI PORT MAP(CLK=>CLK,
						SCLK=>SCLK,
						MOSI=>MOSI,
						SS=>SS,
						MISO=>MISO,
						TX=>DataTX,
						RX=>DataRX
						);
						
ResetEncoder <= TempReset;




Protokol : process(CLk)
variable DummyToggle : STD_LOGIC :='0';
variable TempRX 	 : STD_LOGIC_VECTOR(3 downto 0);
begin

	if rising_edge(CLK) then
		TempReset <= '0';
		--PastDataRX <= DataRX;
															--TempRX := DataRX(N-1 downto N-4); ------------(others => '0');
-----------Pulse Protocol bits--------------------	
		if "1000" /= DataRX(N-1 downto N-4) or "0000" /= DataRX(N-1 downto N-4) then 
			TempRX := DataRX(N-1 downto N-4);
		end if;
---------------------------------------------------
		
		case	TempRX is
			when "1000" => 
								if DummyToggle = '0' then
									TempReset <= '1';
									TempRX := "0000";
									DummyToggle := '1';
								end if;
			when "0100" => Motor1Duty <= DataRX(7 downto 0);
								DataTX <= "00" & Encoder1;
								DummyToggle := '0';
			when "0010" => Motor2Duty <= DataRX(7 downto 0);
								DataTX <= "00" & Encoder2;
								DummyToggle := '0';
			when others => NULL;
		end case;
		Led(7 downto 4) <= TempRX;
		Led(3 downto 0) <= DataRX(N-1 downto N-4);
	end if;
end process;


end Behavioral;

