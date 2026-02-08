#include "app/TransformPipeline.h"

#include "streams/FileInputStream.h"
#include "streams/FileOutputStream.h"

#include "decorators/DecryptInputStream.h"
#include "decorators/EncryptOutputStream.h"
#include "decorators/RleCompressOutputStream.h"
#include "decorators/RleDecompressInputStream.h"

#include <array>
#include <memory>

namespace
{

    std::unique_ptr<IInputDataStream> BuildInputStream(const CliConfig& cfg)
    {
        std::unique_ptr<IInputDataStream> in = std::make_unique<FileInputStream>(cfg.inputPath);

        for (const auto& st : cfg.inputSteps) 
        {
            switch (st.kind) 
            {
            case StepKind::Decompress:
                in = std::make_unique<RleDecompressInputStream>(std::move(in));
                break;
            case StepKind::Decrypt:
                in = std::make_unique<DecryptInputStream>(std::move(in), st.key);
                break;
            default:
                break;
            }
        }

        return in;
    }

    std::unique_ptr<IOutputDataStream> BuildOutputStream(const CliConfig& cfg)
    {
        std::unique_ptr<IOutputDataStream> out = std::make_unique<FileOutputStream>(cfg.outputPath);

        for (auto it = cfg.outputSteps.rbegin(); it != cfg.outputSteps.rend(); ++it) 
        {
            const auto& st = *it;   
            switch (st.kind) 
            {
            case StepKind::Compress:
                out = std::make_unique<RleCompressOutputStream>(std::move(out));
                break;
            case StepKind::Encrypt:
                out = std::make_unique<EncryptOutputStream>(std::move(out), st.key);
                break;
            default:
                break;
            }
        }

        return out;
    }

    void CopyStream(IInputDataStream& in, IOutputDataStream& out)
    {
        std::array<uint8_t, 8192> buf{};

        while (true) 
        {
            std::streamsize got = in.ReadBlock(buf.data(), static_cast<std::streamsize>(buf.size()));

            if (got <= 0)
            {
                break;
            }

            out.WriteBlock(buf.data(), got);
        }
    }

}

int RunTransform(const CliConfig& cfg)
{
    auto in = BuildInputStream(cfg);
    auto out = BuildOutputStream(cfg);

    CopyStream(*in, *out);
    out->Close();

    return 0;
}
