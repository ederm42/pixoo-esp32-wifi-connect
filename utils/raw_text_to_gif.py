from PIL import Image, ImageDraw, ImageFont

def raw_text_to_gif(text, destination_file_path):
    """
    Creates a scrolling GIF from a string
    """
    # GIF settings
    width, height = 16, 16
    font_size = 10  # must fit into 16px height

    # Load font
    try:
        font = ImageFont.truetype("arial.ttf", font_size)
    except:
        font = ImageFont.load_default()

    # Create a temporary image to measure text width
    temp_img = Image.new("RGB", (1, 1))
    draw = ImageDraw.Draw(temp_img)

    # Get text width using modern Pillow method
    bbox = draw.textbbox((0, 0), text, font=font)
    text_width = bbox[2] - bbox[0]

    # Create frames
    frames = []
    for x_offset in range(-width, text_width):
        frame = Image.new("RGB", (width, height), "black")
        draw = ImageDraw.Draw(frame)
        draw.text((-x_offset, 0), text, font=font, fill="white")
        frames.append(frame)

    # Save as GIF
    frames[0].save(
        destination_file_path,
        save_all=True,
        append_images=frames[1:],
        duration=50,  # milliseconds per frame
        loop=0
    )

    print("GIF created successfully!")
    return destination_file_path
