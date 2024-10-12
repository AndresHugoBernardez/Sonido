//
//
//  ♦Andy's Softwares♦
//
// 
//  Estudio de Microsoft Media Foundation
// **********************************************************
// 
// A continuación se mostrará el uso del micrófono para grabar un archivo WAV 
// El objetivo es poder trabajar con muestras del micrófono para futuros programas. 
// 
// 
// Documentación: https://learn.microsoft.com/es-es/windows/win32/medfound/microsoft-media-foundation-sdk
// 
// 
// 
// Nota: Media Foundation es antiguo y es de win32. Su elección fue debida a la cantidad de documentación existente. 
// 
// 
//


///Librerías típicas (obtenidas de ChatGPT)


#include <windows.h>


//******************Librerías de MMF**********************
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>

#include <mfobjects.h>
#include <mfplay.h>
#include <mftransform.h>
#include <mfmediaengine.h>

#include<Mferror.h>
#include<mfcaptureengine.h>




// Biblioteca propia
#include "AndysWAV.h"




//********************************************************
// 
// 
//Típica de C++
#include <iostream>
#include <fstream>


// Biblioteca de otro programa
#include "AndysWAV.h"

// Auxiliar no utilizada.
#include<propvarutil.h>

#pragma comment(lib, "mf.lib")
#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "mfreadwrite.lib")
#pragma comment(lib, "mfuuid.lib")





using namespace std;

///
///PASO 1: Capturar el dispositivo:
/// FUNCIÓN Obtenida de documentación
/// Parámetros:
/// ppSource: Fuente de sonido ej: uno de los micrófonos. 
/// ppReader: Lector al que se asignará ese micrófono
/// Salida: 
/// HRESULT: Ver Mferror.h, también aquí más info: https://learn.microsoft.com/es-es/shows/inside/hresult S_OK si todo salió bien.
HRESULT CreateAudioCaptureDevice(IMFMediaSource** ppSource, IMFSourceReader** ppReader)
{


	*ppSource = NULL;
	UINT32 count = 0,numero=0;
	
	IMFAttributes* pConfig = NULL;
	IMFActivate** ppDevices = NULL;
	// Create an attribute store to hold the search criteria.
	// MFCreateAttributes pertenece a mfapi.h
	// Los atributos van añadiendo contenido a medida que aumenta, ¿se empieza por 1 atributo?
	//std::cout << "\nMFCreaattributes:";
	HRESULT hr = MFCreateAttributes(&pConfig, 0);
	// Request video capture devices.
	//std::cout << hr << "\n";
	if
		(SUCCEEDED(hr))
	{
		//std::cout << "\nSetGUID:";
		// SetGUID: Pertenece a mfobjects.h (include Mfidl.h)
		// El primer parámetro es el atributo a setear, el segundo parametro es el valor a que se desea poner en ese atributo.
		hr = pConfig->SetGUID(
			MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE,
			MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_AUDCAP_GUID // TIPO AUDIO
		);
		//std::cout << hr << "\n";
	}


	hr = pConfig->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);




	// Enumerate the devices,
	if
		(SUCCEEDED(hr))
	{
		//std::cout << "\n MFEnumDeviceSources:";
		hr = MFEnumDeviceSources(pConfig, &ppDevices, &count);
		//std::cout << hr << "))))\n";
	}
	// Create a media source for the first device in the list.
	if
		(SUCCEEDED(hr))
	{
		//std::cout << "\nActivateObject||count:"<<count;


		numero = 0;
		
		if (count > 1) 
		{
			while (numero < 1 && numero < count + 1)
			{
				std::cout << endl << "Tiene " << count << " microfonos. Elija el numero de microfono:";
				std::cin >> numero;

				if (numero < 1 && numero < count + 1)  cout << endl << "El microfono elegido no existe";

			}
			numero--;
		}

		if
			(count > 0)
		{
			
			/// Activación: https://learn.microsoft.com/en-us/windows/win32/api/mfidl/nf-mfidl-mfcreatedevicesourceactivate
			hr = ppDevices[numero]->ActivateObject(IID_PPV_ARGS(ppSource));

			//std::cout << "  hr:" << hr;
		}
		else
		{
			hr = MF_E_NOT_FOUND;
		}
	}


	
	/// Ya activado la fuente y enlazado a ppSource, se libera el puntero que capturó todas. 
	for (DWORD i = 0; i < count; i++)
	{
		ppDevices[i]->Release();
	}
	CoTaskMemFree(ppDevices);
	
	



	return	hr;
}



int main() {

	ofstream OutFile;
	long int tiempo = 3, watchdog = 0;
	unsigned char alto, bajo;

	IMFMediaSource* Fuente = NULL;
	IMFSourceReader* Lector = NULL;
	IMFSample* Sample = NULL;
	IMFMediaBuffer* Buffer = NULL;
	IMFAttributes* pConfig = NULL;
	DWORD k = 0;
	DWORD TamanioBuffer = 0;
	BYTE* audioData = NULL;
	IMFMediaType* pAudioType = NULL;
	LONGLONG tamanio = 0;

	HRESULT hr;
	//ofstream OutFile;
	DWORD N, i, DCurrentSize, DTotalSize = 0, indx = 0, flags = 0;
	long long time = 0;

	BYTE* Data;

	long int Salida = 0;



	std::cout << endl << "Bienvenido:" << endl;

	/// Hay que inicializar la librería COM para que funcione:
	hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (SUCCEEDED(hr)) {
		// Your code here
		//std::cout << "\nCOM library inicializada\n";
	}

	hr = MFStartup(MF_VERSION, MFSTARTUP_NOSOCKET);
	if (SUCCEEDED(hr)) {


		//mainDeOtroProgramaModificado(&OutFile, 5 * 44100);
		//std::cout << "\nCreatingAudio";
	hr = CreateAudioCaptureDevice(&Fuente, &Lector);

	//std::cout << "\nMFCreaattributes:";
	hr = MFCreateAttributes(&pConfig, 2);
	// Request video capture devices.
	//std::cout << hr << "\n";

	if
		(SUCCEEDED(hr))
	{
		//std::cout << "\nSetGUID:";
		// SetGUID: Pertenece a mfobjects.h (include Mfidl.h)
		// El primer parámetro es el atributo a setear, el segundo parametro es el valor a que se desea poner en ese atributo.
		hr = pConfig->SetGUID(
			MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE,
			MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_AUDCAP_GUID // TIPO AUDIO
		);
		//std::cout << hr << "\n";
	}

	if
		(SUCCEEDED(hr))
	{
		hr = pConfig->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);

	}





	if
		(SUCCEEDED(hr)) {
		///Asociando la el Lector (reader) a la fuente(source)
		//std::cout << " MFCreateSourceReader:";
		hr = MFCreateSourceReaderFromMediaSource(Fuente, pConfig, &Lector);
		if (MF_E_DRM_UNSUPPORTED == hr) {
			std::cout << "no se puede usar el micrófono";
		}

		//if (SUCCEEDED(hr)) std::cout << endl << "activado!"; else  std::cout << "\n error:" << hr << "\n";




//Inicializando Tipo de formato de audio
		if (SUCCEEDED(hr)) hr = MFCreateMediaType(&pAudioType);
		if (FAILED(hr)) {
			std::cerr << "Error al crear el tipo de medios." << std::endl;

		}
		else {


			//Preparación del formato del audio: las muestras son PCM 1 canal, 16bits por sample(muestra), frecuencia de 44100Hz
			hr = pAudioType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
			hr = pAudioType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);
			hr = pAudioType->SetUINT32(MF_MT_AUDIO_NUM_CHANNELS, 1);
			hr = pAudioType->SetUINT32(MF_MT_AUDIO_BITS_PER_SAMPLE, 16);
			hr = pAudioType->SetUINT32(MF_MT_AUDIO_SAMPLES_PER_SECOND, 44100);
			hr = Lector->SetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, NULL, pAudioType);

		}




		if (SUCCEEDED(hr)) {

			watchdog = 0;
			i = 0;
			if (InicializarArchivo(&OutFile, &tiempo) == 0) {
				std::cout << endl << endl << "GRABANDO CON MICROFONO DURANTE " << tiempo / 44100 << " SEGUNDOS" << endl << ".";


				while (i < tiempo && watchdog < tiempo * 2) {
					watchdog++;

					
					///Leyendo conjunto de muestras
					///https://learn.microsoft.com/en-us/windows/win32/api/mfreadwrite/nf-mfreadwrite-imfsourcereader-readsample
					hr = Lector->ReadSample(MF_SOURCE_READER_FIRST_AUDIO_STREAM, 0, &indx, &flags, &time, &Sample);


					if (Sample != NULL)
					{




						//
						// Función necesaria para obtener un arreglo de muestras en un buffer.
						hr = Sample->ConvertToContiguousBuffer(&Buffer);


						if (SUCCEEDED(hr)) {

							audioData = NULL;
							TamanioBuffer = 0;

							// Bloquear el buffer para su lectura
							hr = Buffer->Lock(&audioData, NULL, &TamanioBuffer);
							if (SUCCEEDED(hr)) {


							std:cout << ".";

								for (k = 0; k < TamanioBuffer; k = k + 2)

								{
									if (i < tiempo)
									{
										bajo = audioData[k];
										alto = audioData[k + 1];
										OutFile.write((const char*)&bajo, sizeof(unsigned char));
										OutFile.write((const char*)&alto, sizeof(unsigned char));


										OutFile.write((const char*)&bajo, sizeof(unsigned char));
										OutFile.write((const char*)&alto, sizeof(unsigned char));
										i++;
									}
								}




							}


							Buffer->Unlock();
							Buffer->Release();

						}




						Sample->Release();


					}





				}

			}
			if (Sample != NULL) {
				Sample->Release();
				Sample = NULL;
			}

			if (watchdog >= tiempo * 2)std::cout << endl << "ERROR: Hubo problemas en la grabacion!!" << endl;
			std::cout << endl << "...Listo!";
			OutFile.close();
			std::cout << "... Archivo cerrado" << endl << endl;


		}
		else
			std::cout << "error!!!";


		Lector->Release();
		Fuente->Release();

	}
	MFShutdown();
}



		std::system("pause");
	return 0;
}

