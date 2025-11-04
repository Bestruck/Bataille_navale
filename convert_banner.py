#!/usr/bin/env python3
"""
Script pour convertir la bannière SVG en PNG
"""

try:
    from cairosvg import svg2png
    
    # Conversion du SVG en PNG
    svg2png(url='linkedin-banner.svg', 
            write_to='linkedin-banner.png',
            output_width=1584,
            output_height=396)
    
    print("✅ Bannière créée avec succès : linkedin-banner.png")
    print("📏 Dimensions : 1584 x 396 pixels")
    print("📤 Vous pouvez maintenant télécharger ce fichier sur LinkedIn !")
    
except ImportError:
    print("❌ Module 'cairosvg' non installé.")
    print("📦 Installation en cours...")
    import subprocess
    subprocess.run(["pip", "install", "cairosvg"], check=True)
    print("✅ Installation terminée ! Relancez le script.")
