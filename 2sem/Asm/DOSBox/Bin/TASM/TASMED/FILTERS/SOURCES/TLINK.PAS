{*******************************************************}
{ Turbo Link filter for TASMED.                         }
{ Copyright (C) 1996-99 by Eugene Nonko aka CodeMaster. }
{*******************************************************}

{ Output format:
  <message type> [<filename>]([<line number>[,<column number>]]) <message>
  Where message type is "**Error**", "**Fatal**", "*Warning*" or "*Message*" }

program TLink_Filter;

const
  strIFatal = 'Fatal:';
  strIError = 'Error:';
  strIWarning = 'Warning:';
  strInModule = 'in module ';

  strError = '**Error**';
  strFatal = '**Fatal**';
  strWarning = '*Warning*';

  strDPMIError = 'Application load & execute error';

var
  S, MsgType, FName: string;
  P: Byte;

begin
  while not EOF do
  begin
    ReadLn(S);
    if Copy(S, 1, Length(strDPMIError)) = strDPMIError then
      WriteLn(strError, ' () Recofigure your DPMI host, refer to FAQ.COM for details')
    else
    begin
      MsgType := Copy(S, 1, Pos(' ', S)-1);
      if MsgType = strIFatal then MsgType := strFatal else
        if MsgType = strIError then MsgType := strError else
          if MsgType = strIWarning then MsgType := strWarning else
            Continue;
      Delete(S, 1, Pos(' ', S));
      FName := '';
      P := Pos(strInModule, S);
      if P <> 0 then
      begin
        Delete(S, P, Length(strInModule));
        while (S[P] <> ' ') and (Length(S) >= P) do
        begin
          FName := FName+S[P];
          Delete(S, P, 1)
        end;
        Delete(S, P, 1)
      end;
      WriteLn(MsgType, ' ', FName, '() ', S)
    end
  end
end.
