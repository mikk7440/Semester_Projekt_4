
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;



entity Velocity is
port (
		CLK 			: IN STD_LOGIC;
		EncouderIN 	: IN std_logic_vector(13 downto 0);
		Velocity		: OUT std_logic_vector(11 downto 0)
		
		
);
end Velocity;

architecture Behavioral of Velocity is
signal VelocityCLKOut: STD_LOGIC;
signal VelocitySignal: STD_LOGIC_VECTOR(11 downto 0);
signal TempEncouderOut: STD_LOGIC_VECTOR(13 downto 0);
signal NumberToAvg : STD_LOGIC_VECTOR(8 downto 0);
signal RunAvg			: STD_LOGIC;
signal Average			: std_logic_vector(8 downto 0);


function  divide  (a : UNSIGNED; b : UNSIGNED) return UNSIGNED is
variable a1 : unsigned(a'length-1 downto 0):=a;
variable b1 : unsigned(b'length-1 downto 0):=b;
variable p1 : unsigned(b'length downto 0):= (others => '0');
variable i : integer:=0;
begin
	for i in 0 to b'length-1 loop
		p1(b'length-1 downto 1) := p1(b'length-2 downto 0);
		p1(0) := a1(a'length-1);
		a1(a'length-1 downto 1) := a1(a'length-2 downto 0);
		p1 := p1-b1;
			if(p1(b'length-1) ='1') then	
				a1(0) :='0';
				p1 := p1+b1;
			else
				a1(0) :='1';
			end if;
	end loop;
return a1;

end divide;


function  CalVelocity (CounterValue : std_logic_vector) return std_logic_vector is
variable Temp1	: unsigned(CounterValue'length-1 downto 0);
variable result : std_logic_vector(8 downto 0);
constant Const : unsigned(11 downto 0) := "110100000101";
begin
	Temp1 := unsigned(CounterValue);
	result := std_logic_vector(divide(Const,Temp1));
return result;

end CalVelocity;


component VelocityCLK
    port(
		CLK 		: in STD_LOGIC;
		VelocityCLKOut 	: out STD_LOGIC
		);
end component;

component RunningAverage
Port(
		CLK: IN STD_LOGIC;
		Number: IN STD_LOGIC_VECTOR(8 downto 0);
		Average : out STD_LOGIC_VECTOR(8 downto 0);
		run		: IN STD_LOGIC
);
end component;


begin

UM41 : VelocityCLK PORT MAP(
						CLK=>CLK,
						VelocityCLKOut=>VelocityCLKOut
						);

UM42 : RunningAverage PORT MAP(
						CLK=>CLK,
						Number=>NumberToAvg,
						Average=>Average,
						run=>RunAvg
						);


 TempEncouderOut <= EncouderIN;
-------------velocity Process-----------
--process(CLK)
--variable EncoderOld 	: STD_LOGIC_VECTOR(13 downto 0);
--variable VCounter		: std_logic_vector(10 downto 0) := (others => '0');   ---17 
--variable TempToCal	: unsigned(8 downto 0);
--variable DegCalk		: integer range 0 to 20;
--
--constant ConstV : unsigned(14 downto 0) := "111010100110000"; --not 500
--
--begin

process(CLK)
variable EncoderOld 	: STD_LOGIC_VECTOR(13 downto 0);
variable VCounter		: std_logic_vector(11 downto 0) := (others => '0'); 
begin
	if rising_edge(CLK) then
		if EncoderOld /= EncouderIN then 
			--Velocity <= CalVelocity(VCounter);
			Velocity <= VCounter;
			VCounter := (others =>'0');
		else
			if	VelocityCLKOut = '1' then
				VCounter := VCounter +1;
			end if;	
		end if;
		if VCounter = 3000 then 
			VCounter := (others =>'0');
			Velocity <= VCounter(11 downto 0);
		end if;
----------Calculate--------
		
	   
	 
	EncoderOld := EncouderIN;
	end if;
end process;





--	if rising_edge(CLK) then
--		if (VelocityCLKOut = '1') then
--			RunAvg <= '0';
--				if VCounter = 200 then
--					VCounter := (others => '0');				
--					NumberToAvg <= VCounter(8 downto 0);					--NumberToAvg <= (others => '0');
--
--					RunAvg <= '1';
--				else
--					VCounter := VCounter +1;
--					if EncoderOld /= TempEncouderOut then
--					VCounter := VCounter+1;
--						if VCounter < 3 then
--							NumberToAvg <= VCounter(8 downto 0);
--							TempToCal := unsigned(NumberToAvg);--Average);
--							--TempToCal := unsigned(VCounter);
--							VelocitySignal <= std_logic_vector(divide(ConstV,TempToCal));--Average));-- TempToCal));
--							--VelocitySignal <= std_logic_vector(to_unsigned(VCounter, 9));
--							RunAvg <= '1';
--							VCounter := (others => '0');
--						end if;
--					end if;
--					
--				end if; 
--		EncoderOld := TempEncouderOut;
--		Velocity <=VCounter(8 downto 0);--NumberToAvg(8 downto 0);-- Average; --VelocitySignal;--std_logic_vector(TempToCal);--VelocitySignal;--std_logic_vector(TempToCal);-- ;--NumberToAvg; --VelocitySignal;
--		end if;
--	end if;
--end process;




end Behavioral;
